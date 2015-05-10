#include "renderer.h"


void* renderer ( void *arg )
{

	logWrite ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;

	glfwMakeContextCurrent ( context->window );
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	glUseProgram (context->shader_program);

	context->ratio = ((float)context->screen_width)/(float)context->screen_height;

	float last_time=127.f;
	struct timespec time1;
	struct timespec time2;

	int i;

	while (context->running)
	{
		if (glfwWindowShouldClose (context->window)) context->running = 0;

		//cap to 60 fps, be resource friendly my friend
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
		//printf ("%f\n", fps);


		mat4x4 WMat;
		mat4x4 PVMat;
		mat4x4 viewMat;
		mat4x4 perMat;
		GLuint vpID;
		GLuint wID;

		glViewport (0, 0, context->screen_width, context->screen_height);
		glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4x4_look_at(viewMat, context->camera->eye, context->camera->target, context->camera->up);
		mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
		mat4x4_mul (PVMat, perMat, viewMat);
		vpID = glGetUniformLocation(context->shader_program, "VP");
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		glBindVertexArray (context->dcube_mesh->vao_id);
		Step* step = context->snake->solutions->head->step;
		for ( i=0; i <= context->snake->currentUnit; i++ )
		{
			if (i%2==0)
				glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else		
				glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

			mat4x4_translate( WMat, step[i].coord.x-1, step[i].coord.y-1, step[i].coord.z-1 );

			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, (0.85f+ 0.3f*abs(cos(4*glfwGetTime()))) );
			else
				mat4x4_scale3d(WMat, WMat, 0.9f);

			wID = glGetUniformLocation(context->shader_program, "W");
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, context->dcube_mesh->nb_faces);
			//glDrawElements(GL_QUADS, 4 * context->dcube_mesh->nb_faces, GL_UNSIGNED_BYTE, context->dcube_mesh->indices);
		}
		
		glViewport (-20, -240, context->screen_height, context->screen_height);
		mat4x4_identity(viewMat);
		mat4x4_identity(perMat);
		mat4x4_identity(WMat);
		mat4x4_scale3d(WMat, WMat, 0.05f);
		mat4x4_mul (PVMat, perMat, viewMat);
		vpID = glGetUniformLocation(context->shader_program, "VP");
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);


		int xdir = 0;
		int ydir = 1;
		float r_angle =  -M_PI/4;
		for ( i=0; i <= context->snake->length-1; i++ )
		{
			mat4x4_rotate_Z(WMat, WMat, r_angle);

			if (i%2==0)
				glBindTexture(GL_TEXTURE_2D, context->lwoodtex);
			else		
				glBindTexture(GL_TEXTURE_2D, context->dwoodtex);

			mat4x4_translate_in_place( WMat, xdir, ydir, 0);
			
		
			mat4x4_scale3d(WMat, WMat, 0.9f);

			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, ((0.9f*abs(cos(4*glfwGetTime())))) );
			wID = glGetUniformLocation(context->shader_program, "W");
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, context->dcube_mesh->nb_faces);
			//glDrawElements(GL_QUADS, 4 * context->dcube_mesh->nb_faces, GL_UNSIGNED_BYTE, context->dcube_mesh->indices);
			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, (1/(0.9f*abs(cos(4*glfwGetTime())))) );


			mat4x4_rotate_Z(WMat, WMat, -r_angle);
			
			mat4x4_scale3d(WMat, WMat, 1.1f);
			if (context->snake->units[i]==CORNER)
			{
				xdir = (xdir==0?1:0);
				ydir = (ydir==0?1:0);
			}
		
		}
		

		glfwSwapBuffers (context->window);
	}

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
};
