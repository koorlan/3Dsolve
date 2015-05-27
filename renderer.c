#include "renderer.h"

void* renderer ( void *arg )
{
	//! [1] OpenGl setup
	logWrite ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;

	glfwMakeContextCurrent ( context->window );
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	context->ratio = ((float)context->screen_width)/(float)context->screen_height;

	mat4x4 WMat;
	mat4x4 PVMat;
	mat4x4 viewMat;
	mat4x4 perMat;
	GLuint vpID = glGetUniformLocation(context->snake_program, "VP");
	GLuint vpID2 = glGetUniformLocation(context->picking_program, "VP");
	GLuint wID = glGetUniformLocation(context->snake_program, "W");
	GLuint pickcolorID = glGetUniformLocation(context->picking_program, "colorID");
	GLuint timeID = glGetUniformLocation(context->snake_program, "time");
	GLuint alphaID = glGetUniformLocation(context->snake_program, "alpha");

	float last_time=127.f;
	float accumulator;
	struct timespec time1;
	struct timespec time2;
	//! [1]

	//! [2] Player initialization
	Player* curPlayer;
	if (context->playmode == PM_PLAY)
		curPlayer = gplayer;
	else
		curPlayer = gsolver;

	int i;
	int cubesNb = context->snake->length;
	//! [2]

	//! [3] Renderer loop
	while (context->running)
	{
		if (glfwWindowShouldClose (context->window)) context->running = 0;

		//! [4] FPS limitation control
		float cur_time = glfwGetTime ();
		float fps = 1/((cur_time-last_time));
		if (fps>60.0f)
		{
			time1.tv_sec = 0;
			time1.tv_nsec = 1000;
			nanosleep (&time1, &time2);
			continue;
		}
		else last_time = cur_time;
		//printf ("%f\n", fps); display fps
		//! [4]

		// Set OpenGl viewport to the entire window
		glViewport (0, 0, context->screen_width, context->screen_height);

		if (context->playmode == PM_PLAY)
			curPlayer = gplayer;
		else
			curPlayer = gsolver;

		//! [5] Color picking (cube selection)
		curPlayer->segEnd = cubesNb;
		if (curPlayer->selected!=-1 && curPlayer->selected!=cubesNb)
			for ( i=curPlayer->selected+1; i < cubesNb; i++ )
				if ( context->snake->units[i] == CORNER)
				{
					curPlayer->segEnd = i;
					break;
				}
		// The selected segment goes from gplayer->selected to segEnd

		if ( context->drawpick == 1 )
		{
			//==========render cubes==========
			glClearColor( 1.0f, 1.0f, 1.0f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram (context->picking_program);
			mat4x4_look_at(viewMat, context->camera->eye,
					context->camera->target, context->camera->up);
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID2, 1, GL_FALSE, &PVMat[0][0]);
			glBindVertexArray (context->cube_mesh->vao_id);

			for ( i=0; i <= curPlayer->segEnd; i++ )
			{
				glUniform3f(pickcolorID, ((float)i)/255.f, 255.f, 0.f);

				mat4x4_identity ( WMat );
				mat4x4_mul (WMat, WMat, curPlayer->realCubePos[i]);
				mat4x4_mul (WMat, WMat, curPlayer->realCubeRot[i]);
				if (context->spread == 1) mat4x4_scale3d(WMat, WMat, 0.6f);

				glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}
			//==========render menu button==========
			mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glBindVertexArray (app->menu->mesh->vao_id);
			glUniform1f(alphaID, 1.0f);
			float xoffset = -1.f  + app->menu->margin[0];
			float yoffset = 1.f  - app->menu->bboxRel[1] - app->menu->margin[1];

			mat4x4_identity(WMat);
			//mat4x4_scale_aniso(WMat, WMat, 1/context->ratio, 1.f, 0.f);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);

			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

			accumulator = 0.f;
			if(app->menu->type == COLLUM){
					for ( i = 0; i < app->menu->size; i++) {
						glUniform3f(pickcolorID,255.f, ((float)i)/255.f, 0.f);
						mat4x4_identity(WMat);
						mat4x4_scale3d(WMat, WMat, 1.f);
						mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
						if (i == 0)
							accumulator =  app->menu->bboxRel[1] - app->menu->item[i]->bboxRel[1] - 2*(app->menu->item[i]->margin[1]/context->screen_height);
						else
							accumulator -= 2*(app->menu->item[i]->margin[1]/context->screen_height);
						mat4x4_translate_in_place( WMat, 0.f ,accumulator , 0);
						mat4x4_scale_aniso(WMat, WMat, 1.f, app->menu->item[i]->bboxRel[1]/app->menu->bboxRel[1] * 2.f, 0.f); //2.f est le coef de grossisement de la hitbox
						glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
						glDrawArrays(GL_TRIANGLES, 0,app->menu->mesh->nb_faces);
						accumulator -= (app->menu->item[i]->bboxRel[1] - app->menu->item[i]->bboxRel[3]) + 2*(app->menu->item[i]->margin[3]/context->screen_height);
					}
			}

			glFlush();
			glFinish();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			unsigned char data[4];
			glReadPixels(gxpos, context->screen_height-gypos, 1, 1,
					GL_RGBA, GL_UNSIGNED_BYTE, data);
			//if (data[0] == gplayer->selected)
			//	playerRotate(gplayer, gplayer->selected, context->snake);
			if( (data[1]==255?-1:data[1]) != -1)
				app->menu->selected = (data[1]==255?-1:data[1]);
			else
				gplayer->selected = (data[0]==255?-1:data[0]);
			printf ("cube %d gplayer->selected\n", gplayer->selected);

			context->drawpick = 0;
		}
		//! [5]

		//==========view cubes==========
		glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (context->snake_program);
		mat4x4_look_at(viewMat, context->camera->eye,
				context->camera->target, context->camera->up);
		mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
		mat4x4_mul (PVMat, perMat, viewMat);
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
		glBindVertexArray (context->cube_mesh->vao_id);

		for ( i=0; i < cubesNb; i++ )
		{
			glUniform1f(timeID, glfwGetTime());
			glUniform1f(alphaID, (i > curPlayer->segEnd?0.2f:1.0f));

			mat4x4_identity ( WMat );
			mat4x4_mul (WMat, WMat, curPlayer->realCubePos[i]);
			mat4x4_mul (WMat, WMat, curPlayer->realCubeRot[i]);
			if (context->spread == 1) mat4x4_scale3d(WMat, WMat, 0.6f);
			if (curPlayer->selected == i )
				mat4x4_scale3d(WMat, WMat, 0.8f + (0.2f * abs(cos(4*(glfwGetTime())))));
			else mat4x4_scale3d(WMat, WMat, 0.97f);

			glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

			//pair/impair = blanc/noir
			if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

			glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);

		}

		//==========view snake 2D==========
		mat4x4_identity(viewMat);
		mat4x4_identity(perMat);
		mat4x4_mul (PVMat, perMat, viewMat);
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
		glBindVertexArray (context->square_mesh->vao_id);
		glUniform1f(alphaID, 1.0f);

		float xoffset = -(context->snake->length*0.3333333f);
		float yoffset = -0.75f * 20.f;
		float r_angle =  -M_PI/4;
		for ( i=0; i <= context->snake->length-1; i++ )
		{
			if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

			mat4x4_identity(WMat);
			mat4x4_scale_aniso(WMat, WMat, 1/context->ratio, 1.f, 0.f);
			mat4x4_scale3d(WMat, WMat, 0.05f);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_rotate_Z(WMat, WMat, -r_angle);
			mat4x4_translate_in_place( WMat, curPlayer->flatCubePos[i][0], curPlayer->flatCubePos[i][2], 0);
			if (i==curPlayer->selected)
				mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, context->square_mesh->nb_faces);
		}

		//==========view Menu template==========

		 	mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glBindVertexArray (app->menu->mesh->vao_id);
			glUniform1f(alphaID, 1.0f);
			xoffset = -1.f  + app->menu->margin[0];
			yoffset = 1.f  - app->menu->bboxRel[1] - app->menu->margin[1];
			r_angle =  0.f;

			glBindTexture(GL_TEXTURE_2D, context->menutex);
			mat4x4_identity(WMat);

			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_scale_aniso(WMat, WMat, app->menu->bboxRel[0], app->menu->bboxRel[1] , 0.f);
			mat4x4_rotate_Z(WMat, WMat, -r_angle);

			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0,app->menu->mesh->nb_faces);

			//float accumulator = app->menu->bboxRel[1] - app->menu->item[0]->bboxRel[1] - 2*(app->menu->item[0]->margin[1]/context->screen_height) ;
			accumulator = 0.f;
			if(app->menu->type == COLLUM){
					for ( i = 0; i < app->menu->size; i++) {
						mat4x4_identity(WMat);
						mat4x4_scale3d(WMat, WMat, 1.f);
						mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
						if (i == 0)
				      accumulator =  app->menu->bboxRel[1] - app->menu->item[i]->bboxRel[1] - 2*(app->menu->item[i]->margin[1]/context->screen_height);
				    else
				      accumulator -= 2*(app->menu->item[i]->margin[1]/context->screen_height);
				    mat4x4_translate_in_place( WMat, 0.f ,accumulator , 0);
						mat4x4_scale_aniso(WMat, WMat, app->menu->bboxRel[0], app->menu->bboxRel[1] , 0.f);
						mat4x4_scale_aniso(WMat, WMat, 1.f, (app->menu->item[i]->bboxRel[1]/app->menu->bboxRel[1]) , 0.f);
						glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
						if(i == app->menu->selected){
							glBindTexture(GL_TEXTURE_2D, context->itemtex);
						//	glDrawArrays(GL_TRIANGLES, 0, app->menu->mesh->nb_faces);
							glBindTexture(GL_TEXTURE_2D, context->menutex);
							accumulator -= (app->menu->item[i]->bboxRel[1] - app->menu->item[i]->bboxRel[3]) + 2*(app->menu->item[i]->margin[3]/context->screen_height);
							continue;
						}
						glBindTexture(GL_TEXTURE_2D, context->itemtex);
						glDrawArrays(GL_TRIANGLES, 0, app->menu->mesh->nb_faces);
						accumulator -= (app->menu->item[i]->bboxRel[1] - app->menu->item[i]->bboxRel[3]) + 2*(app->menu->item[i]->margin[3]/context->screen_height);
					}
			}


		//==========view Text==========
		if (!pthread_mutex_trylock(app->menu->mutex)){
			glUseProgram (0);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glViewport (0, 0, context->screen_width, context->screen_height);

			accumulator = 0.f;
			glColor4f (1.f,0.f,0.f,1.f);

			glTranslatef( (-1.f  + app->menu->margin[0]) + app->menu->bboxRel[0] , (1.f -  app->menu->margin[1]),0.f);
			glOrtho(-context->screen_width/2,context->screen_width/2,-context->screen_height/2,context->screen_height/2,0,1);

			for ( i = 0;  i < app->menu->size; i++) {

			  glColor4f (app->menu->item[i]->descriptor.color.r, app->menu->item[i]->descriptor.color.g,app->menu->item[i]->descriptor.color.b,app->menu->item[i]->descriptor.color.a);
				glTranslatef(app->menu->item[i]->descriptor.bbox[0],-(app->menu->item[i]->margin[1] + app->menu->item[i]->descriptor.bbox[4]),0.f);
			  accumulator += app->menu->item[i]->descriptor.bbox[4] - app->menu->item[i]->descriptor.bbox[1]  + app->menu->item[i]->margin[1] + app->menu->item[i]->margin[3];
			  ftglRenderFont(app->menu->item[i]->descriptor.font, app->menu->item[i]->descriptor.name, FTGL_RENDER_ALL);
				glTranslatef(-app->menu->item[i]->descriptor.bbox[0],-(app->menu->item[i]->margin[3] + abs(app->menu->item[i]->descriptor.bbox[1]) ),0.f);
			}


			pthread_mutex_unlock(app->menu->mutex);
		}

		glfwSwapBuffers (context->window);
	}
	//! [3]

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
}
