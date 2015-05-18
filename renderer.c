#include "renderer.h"

void* renderer ( void *arg )
{

	logWrite ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;

	glfwMakeContextCurrent ( context->window );
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	context->ratio = ((float)context->screen_width)/(float)context->screen_height;

	int i;

	mat4x4 WMat;
	mat4x4 PVMat;
	mat4x4 viewMat;
	mat4x4 perMat;
	GLuint vpID = glGetUniformLocation(context->snake_program, "VP");
	GLuint vpID2 = glGetUniformLocation(context->picking_program, "VP");
	GLuint wID = glGetUniformLocation(context->snake_program, "W");
	GLuint pickcolorID = glGetUniformLocation(context->picking_program, "colorID");
	GLuint timeID = glGetUniformLocation(context->snake_program, "time");

	vec3 vol_offset;
	vol_offset[0] = (context->snake->volume.max.x%2==0 ? context->snake->volume.max.x /2 - 0.5f : (context->snake->volume.max.x -1 )/2);
	vol_offset[1] = (context->snake->volume.max.y%2==0 ? context->snake->volume.max.y /2 - 0.5f : (context->snake->volume.max.y -1 )/2);
	vol_offset[2] = (context->snake->volume.max.z%2==0 ? context->snake->volume.max.z /2 - 0.5f : (context->snake->volume.max.z -1 )/2);

	context->camera->distance = 4.f;
	int selected = -1;

	while (context->running)
	{
		if (glfwWindowShouldClose (context->window)) context->running = 0;


		glViewport (0, 0, context->screen_width, context->screen_height);


		if (context->drawpick==1)
		{
			glClearColor( 1.f, 1.f, 1.f, 1.f );
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			int objID = 0;

			glUseProgram (context->picking_program);

			mat4x4_look_at(viewMat, context->camera->eye, context->camera->target, context->camera->up);
			mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
			mat4x4_mul (PVMat, perMat, viewMat);
			glUniformMatrix4fv(vpID2, 1, GL_FALSE, &PVMat[0][0]);

			glBindVertexArray (context->cube_mesh->vao_id);

			Step* step = context->snake->solutions->head->step;

			for ( i=0; i <= context->snake->currentUnit; i++ )
			{
				if (i%2==0)
					glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
				else
					glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

				glUniform3f(pickcolorID, ((float)objID)/255.f, 0.f, 0.f);
				objID++;

				mat4x4_translate( WMat, step[i].coord.x-vol_offset[0], step[i].coord.y-vol_offset[1], step[i].coord.z-vol_offset[2] );

				if (i==context->snake->currentUnit)
					mat4x4_scale3d(WMat, WMat, (0.85f+ 0.3f*abs(cos(4*glfwGetTime()))) );
				else if (i==selected)
					mat4x4_scale3d(WMat, WMat, 0.8f);
				else
					mat4x4_scale3d(WMat, WMat, 0.9f);

				glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
			}

			glFlush();
			glFinish();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			unsigned char data[4];
			glReadPixels(gxpos, context->screen_height-gypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			selected = (data[0]==255?selected:data[0]);
			printf ("cube %d selected\n", selected);

			context->drawpick = 0;
		}

		glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram (context->snake_program);


		mat4x4_look_at(viewMat, context->camera->eye, context->camera->target, context->camera->up);
		mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
		mat4x4_mul (PVMat, perMat, viewMat);
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		glUniform1f(timeID, glfwGetTime());

		glBindVertexArray (context->cube_mesh->vao_id);

		Step* step = context->snake->solutions->head->step;

		for ( i=0; i <= context->snake->currentUnit; i++ )
		{
			if (i%2==0)
				glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else
				glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

			mat4x4_translate( WMat, step[i].coord.x-vol_offset[0], step[i].coord.y-vol_offset[1], step[i].coord.z-vol_offset[2] );

			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, (0.85f+ 0.3f*abs(cos(4*glfwGetTime()))) );
			else if (i==selected)
				mat4x4_scale3d(WMat, WMat, 0.8f);
			else
				mat4x4_scale3d(WMat, WMat, 0.9f);

			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, context->cube_mesh->nb_faces);
		}


		mat4x4_identity(viewMat);
		mat4x4_identity(perMat);
		mat4x4_identity(WMat);
		mat4x4_scale_aniso(WMat, WMat, 1/context->ratio, 1.f, 0.f);
		mat4x4_scale3d(WMat, WMat, 0.05f);
		mat4x4_mul (PVMat, perMat, viewMat);
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		glBindVertexArray (context->square_mesh->vao_id);

		int xdir = 0;
		int ydir = 1;
		float xoffset = -(context->snake->length*0.333333f);
		float yoffset = -0.75f * 20.f;
		float r_angle =  -M_PI/4;
		for ( i=0; i <= context->snake->length-1; i++ )
		{
			if (i%2==0) glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

			mat4x4_translate_in_place( WMat, xoffset, yoffset, 0);
			mat4x4_rotate_Z(WMat, WMat, r_angle);
			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
			//pushmatrix
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, (1/(0.9f*abs(cos(4*glfwGetTime())))) );
			mat4x4_translate_in_place( WMat, xdir, ydir, 0);
			mat4x4_rotate_Z(WMat, WMat, -r_angle);
			mat4x4_translate_in_place( WMat, -xoffset, -yoffset, 0);

			glDrawArrays(GL_TRIANGLES, 0, context->square_mesh->nb_faces);


			if (context->snake->units[i]==CORNER)
			{
				xdir = (xdir==0?1:0);
				ydir = (ydir==0?1:0);
			}

		}

		glViewport (0, 0, context->screen_width, context->screen_height);
		glUseProgram (0);
		glRasterPos2f( -0.98f, 0.92f);
		glColor4f (0.2f+abs(cos(20*glfwGetTime())), 0.2f+abs(cos(10*glfwGetTime())), 0.2f+abs(cos(15*glfwGetTime())), 0.2f+abs(cos(5*glfwGetTime())));
		//glRotatef(1.f,0.f,0.f,0.f);
		ftglSetFontFaceSize(myfont, 20, 72);
    ftglRenderFont(myfont, "Snake resolver v0.1b.70", FTGL_RENDER_ALL);

		glfwSwapBuffers (context->window);
	}

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
};
