#include "renderer.h"

void* renderer ( void *arg )
{
	/// [1] OpenGl setup
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
	struct timespec time1;
	struct timespec time2;
	/// [1]

	/// [2] Player initialization
	Player* curPlayer;
	if (context->playmode == PM_PLAY)
		curPlayer = gplayer;
	else
		curPlayer = gsolver;

	int i;
	int cubesNb = context->snake->length;


	vec3 * flatCubePos = malloc ( cubesNb * 3 * sizeof(float) );
	for (i=0;i<cubesNb;i++)
	{
		flatCubePos[i][0] = (float) curPlayer->steps[i].coord.x;
		flatCubePos[i][1] = (float) curPlayer->steps[i].coord.y;
		flatCubePos[i][2] = (float) curPlayer->steps[i].coord.z;
	}
	/// [2]

	/// [3] Renderer loop
	while (context->running)
	{
		if (glfwWindowShouldClose (context->window)) context->running = 0;

		/// [4] FPS limitation control
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
		/// [4]

		// Set OpenGl viewport to the entire window
		glViewport (0, 0, context->screen_width, context->screen_height);

		if (context->playmode == PM_PLAY)
			curPlayer = gplayer;
		else
			curPlayer = gsolver;

		/// [5] Color picking (cube selection)
		int segEnd = cubesNb;
		if (curPlayer->selected!=-1 && curPlayer->selected!=cubesNb)
			for ( i=curPlayer->selected+1; i < cubesNb; i++ )
				if ( context->snake->units[i] == CORNER)
				{
					segEnd = i;
					break;
				}
		// The selected segment goes from gplayer->selected to segEnd

		if ( context->drawpick == 1 )
		{
			glClearColor( 1.0f, 1.0f, 1.0f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram (context->picking_program);
			mat4x4_look_at(viewMat, context->camera->eye,
					context->camera->target, context->camera->up);
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID2, 1, GL_FALSE, &PVMat[0][0]);
			glBindVertexArray (context->cube_mesh->vao_id);

			int objID = 0;
			for ( i=0; i <= segEnd; i++ )
			{
				glUniform3f(pickcolorID, ((float)objID)/255.f, 0.f, 0.f);
				objID++;

				mat4x4_identity ( WMat );
					mat4x4_translate ( WMat, curPlayer->steps[i].coord.x,
								 curPlayer->steps[i].coord.y,
								 curPlayer->steps[i].coord.z );

				glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}
			glFlush();
			glFinish();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			unsigned char data[4];
			glReadPixels(gxpos, context->screen_height-gypos, 1, 1,
					GL_RGBA, GL_UNSIGNED_BYTE, data);
			//if (data[0] == gplayer->selected)
			//	playerRotate(gplayer, gplayer->selected, context->snake);
			gplayer->selected = (data[0]==255?-1:data[0]);
			printf ("cube %d gplayer->selected\n", gplayer->selected);

			context->drawpick = 0;

		}
		/// [5]

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

		segEnd = cubesNb;
		if (curPlayer->selected!=-1 && curPlayer->selected!=cubesNb)
			for ( i=curPlayer->selected+1; i < cubesNb; i++ )
				if ( context->snake->units[i] == CORNER)
				{
					segEnd = i;
					break;
				}

		for ( i=0; i < cubesNb; i++ )
		{


			glUniform1f(timeID, glfwGetTime());
			glUniform1f(alphaID, (i > segEnd?0.2f:1.0f));

			mat4x4_identity ( WMat );
			mat4x4_translate ( WMat, curPlayer->steps[i].coord.x,
						 curPlayer->steps[i].coord.y,
						 curPlayer->steps[i].coord.z );
				if (curPlayer->selected > -1 && i>=curPlayer->selected && i<=segEnd)
					mat4x4_scale3d(WMat, WMat, 0.8f + (0.2f * abs(cos(4*(glfwGetTime()+i)))));
				else mat4x4_scale3d(WMat, WMat, 0.97f);

			glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

			//pair/impair = blanc/noir
			if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

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
			if (i%2!=0) glBindTexture(GL_TEXTURE_2D, context->dwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->lwoodtex);

			mat4x4_identity(WMat);
			mat4x4_scale_aniso(WMat, WMat, 1/context->ratio, 1.f, 0.f);
			mat4x4_scale3d(WMat, WMat, 0.05f);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_rotate_Z(WMat, WMat, -r_angle);
			mat4x4_translate_in_place( WMat, flatCubePos[i][0], flatCubePos[i][2], 0);
			if (i==curPlayer->selected)
				mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, context->square_mesh->nb_faces);
		}

		//==========view Menu template==========
		// do stuff...
		 	mat4x4_identity(viewMat);
			mat4x4_identity(perMat);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);
			glBindVertexArray (mymenu->mesh->vao_id);
			glUniform1f(alphaID, 1.0f);
			xoffset = -1.f + (mymenu->bbox[2]-mymenu->bbox[0])/context->screen_width ;//+ mymenu->margin[0];
			yoffset = 1.f - (mymenu->bbox[3]-mymenu->bbox[1])/context->screen_height ;//- mymenu->margin[1] ;
			r_angle =  0.f;

			glBindTexture(GL_TEXTURE_2D, 0);
			mat4x4_identity(WMat);
			//mat4x4_scale_aniso(WMat, WMat, 1/context->ratio, 1.f, 0.f);
			mat4x4_scale3d(WMat, WMat, 1.f);
			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_rotate_Z(WMat, WMat, -r_angle);
			//mat4x4_translate_in_place( WMat, flatCubePos[i][0], flatCubePos[i][2], 0);
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0,mymenu->mesh->nb_faces);

		//==========view Text==========
		if (!pthread_mutex_trylock(mymenu->mutex)){
			glUseProgram (0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glViewport (0, 0, context->screen_width, context->screen_height);
			float acc = 0.f;

			for ( i = 0;  i < mymenu->size; i++) {
				glLoadIdentity();
				glOrtho(0,context->screen_width,0,context->screen_height,0,1);		//Render menu template
				glColor4f (mymenu->item[i]->descriptor.color.r, mymenu->item[i]->descriptor.color.g,mymenu->item[i]->descriptor.color.b,mymenu->item[i]->descriptor.color.a);

				glTranslatef((mymenu->margin[0]) + mymenu->bbox[0] + mymenu->item[i]->descriptor.bbox[0] + mymenu->item[i]->margin[0],(context->screen_height - mymenu->margin[1]) - acc  - mymenu->item[i]->descriptor.bbox[4] - mymenu->item[i]->margin[1]  ,0.f);
				acc += mymenu->item[i]->descriptor.bbox[4] - mymenu->item[i]->descriptor.bbox[1]  + mymenu->item[i]->margin[1] + mymenu->item[i]->margin[3];
				ftglRenderFont(mymenu->item[i]->descriptor.font, mymenu->item[i]->descriptor.name, FTGL_RENDER_ALL);
			}
			pthread_mutex_unlock(mymenu->mutex);
		}

		glfwSwapBuffers (context->window);
	}
	/// [3]

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
}


void dir2vec ( Dir dir, vec3 vec )
{
	int dim;
	for (dim=0; dim<3; dim++) vec[dim] = 0.f;

	switch ( dir )
	{
		case UP:
			vec[1] = 1.f;
			break;
		case DOWN:
			vec[1] = -1.f;
			break;
		case LEFT:
			vec[0] = -1.f;
			break;
		case RIGHT:
			vec[0] = 1.f;
			break;
		case FRONT:
			vec[2] = 1.f;
			break;
		case BACK:
			vec[2] = -1.f;
			break;
		case DNONE:
		default:
			break;
	}

}
