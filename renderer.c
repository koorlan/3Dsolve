#include "renderer.h"

void* renderer ( void *arg )
{

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


	int i;
	int cubesNb = context->snake->length;

	gplayer = playerInit ( context->snake );
	vec3 * flatCubePos = malloc ( cubesNb * 3 * sizeof(float) );
	for (i=0;i<cubesNb;i++)
	{
		flatCubePos[i][0] = (float) gplayer->steps[i].coord.x;
		flatCubePos[i][1] = (float) gplayer->steps[i].coord.y;
		flatCubePos[i][2] = (float) gplayer->steps[i].coord.z;
	}

	float last_time=127.f;
	struct timespec time1;
	struct timespec time2;

	while (context->running)
	{
		if (glfwWindowShouldClose (context->window)) context->running = 0;

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

		glViewport (0, 0, context->screen_width, context->screen_height);


		//crap
		if (context->flatten == 1)
		{
			playerFlatten ( gplayer, context->snake, 0 );
			context->flatten = 0;
		}

		//===========picking==========

		int segEnd = cubesNb;
		if (gplayer->selected!=-1 && gplayer->selected!=cubesNb)
			for ( i=gplayer->selected+1; i < cubesNb; i++ )
				if ( context->snake->units[i] == CORNER)
				{
					segEnd = i;
					break;
				}

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
				mat4x4_translate ( WMat, gplayer->steps[i].coord.x, gplayer->steps[i].coord.y, gplayer->steps[i].coord.z );
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
		if (gplayer->selected!=-1 && gplayer->selected!=cubesNb)
			for ( i=gplayer->selected+1; i < cubesNb; i++ )
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
			mat4x4_translate ( WMat, gplayer->steps[i].coord.x, gplayer->steps[i].coord.y, gplayer->steps[i].coord.z );
			if (i==gplayer->selected) mat4x4_scale3d(WMat, WMat, 0.8f + (0.2f * abs(cos(4*glfwGetTime()))));
			else mat4x4_scale3d(WMat, WMat, 0.97f);
			glUniformMatrix4fv ( wID, 1, GL_FALSE, &WMat[0][0] );

			//pair/impair = blanc/noir
			if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

			glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);

		}

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
			mat4x4_translate_in_place( WMat, flatCubePos[i][0], flatCubePos[i][2], 0);
			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, context->square_mesh->nb_faces);
		}

		glUseProgram (0);
		glLoadIdentity();
		//Render text over menu template
		glViewport (0, 0, context->screen_width, context->screen_height);
		glOrtho(0,context->screen_width,0,context->screen_height,0,1);		//Render menu template
		glColor4f(1.f/255.f,130.f/255.f,255.f/255.f,0.3+0.3*(1.f-sin(glfwGetTime())));
		glBegin(GL_QUADS);
	   	glVertex3f( (mymenu->margin[0]) + mymenu->bbox[0], (context->screen_height-mymenu->margin[1]) - mymenu->bbox[1],0.f);
	   	glVertex3f( (mymenu->margin[0]) + mymenu->bbox[0], (context->screen_height-mymenu->margin[1]) - mymenu->bbox[3],0.f);
	   	glVertex3f( (mymenu->margin[0]) + mymenu->bbox[2], (context->screen_height-mymenu->margin[1]) - mymenu->bbox[3],0.f);
	   	glVertex3f( (mymenu->margin[0]) + mymenu->bbox[2], (context->screen_height-mymenu->margin[1]) - mymenu->bbox[1],0.f);
		glEnd();
		//glColor4f(0.0f,0.0f,1.0f,0.4f);
		//glBegin(GL_QUADS);
		//glVertex3f(mymenu->bbox[0],mymenu->bbox[1],0.f);
		//glVertex3f(mymenu->bbox[0],mymenu->bbox[3],0.f);
		//glVertex3f(mymenu->bbox[2],mymenu->bbox[3],0.f);
		//glVertex3f(mymenu->bbox[2],mymenu->bbox[1],0.f);
		//glEnd();

		float acc = 0.f;

		for ( i = 0;  i < mymenu->size; i++) {
			glLoadIdentity();
			glOrtho(0,context->screen_width,0,context->screen_height,0,1);		//Render menu template

			//glRotatef(360.f*abs(cos(1.f*glfwGetTime())),0.f,0.f,1.f);
			glColor4f (mymenu->item[i]->descriptor.color.r, mymenu->item[i]->descriptor.color.g,mymenu->item[i]->descriptor.color.b,0.3+0.3*(1.f-sin(glfwGetTime())));

			glTranslatef((mymenu->margin[0]) + mymenu->bbox[0] + mymenu->item[i]->descriptor.bbox[0] + mymenu->item[i]->margin[0],(context->screen_height-mymenu->margin[1]) - acc  - mymenu->item[i]->descriptor.bbox[4] - mymenu->item[i]->margin[1]  ,0.f);
			acc += mymenu->item[i]->descriptor.bbox[4] - mymenu->item[i]->descriptor.bbox[1]  + mymenu->item[i]->margin[1];
			ftglSetFontCharMap(mymenu->item[i]->descriptor.font, mymenu->item[i]->descriptor.fontSize);
			ftglRenderFont(mymenu->item[i]->descriptor.font, mymenu->item[i]->descriptor.name, FTGL_RENDER_ALL);
			//buildMenu(mymenu,context->screen_width,context->screen_height);
		}
		//calcMenu(mymenu);

  	//ftglRenderFont(myfont, "Snake resolver v0.1b.70", FTGL_RENDER_ALL);


		glfwSwapBuffers (context->window);
	}

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
};


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
