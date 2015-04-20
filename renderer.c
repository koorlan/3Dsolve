#include "renderer.h"

//TODO: camera
//TODO: snake du dessous
//TODO: cube en surbrillance

void* renderer ( void *arg )
{

	logWrite ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;

	glfwMakeContextCurrent ( context->window );
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glPointSize (4.0f);

	glUseProgram (context->shader_program);

	glViewport (0, 0, context->screen_width, context->screen_height);
	context->ratio = ((float)context->screen_width)/(float)context->screen_height;

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
		//printf ("%f\n", fps);

		mat4x4 WMat;
		mat4x4 PVMat;
		mat4x4 viewMat;
		mat4x4 perMat;
		GLuint vpID;
		GLuint wID;

		mat4x4_look_at(viewMat, context->camera->eye, context->camera->target, context->camera->up);
		mat4x4_perspective(perMat, context->camera->fov, context->ratio, F_NEAR, F_FAR);
		mat4x4_mul (PVMat, perMat, viewMat);
		vpID = glGetUniformLocation(context->shader_program, "VP");
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int i;
		Step* step = context->snake->solutions->head->step;
		for ( i=0; i <= context->snake->currentUnit; i++ )
		{
			if (i%2==0)
				glBindVertexArray (context->dcube_mesh->vao_id);
			else glBindVertexArray (context->lcube_mesh->vao_id);

			mat4x4_translate( WMat, step[i].coord.x-1, step[i].coord.y-1, step[i].coord.z-1 );
			if (i==context->snake->currentUnit)
				mat4x4_scale3d(WMat, WMat, (0.85f+ 0.3f*abs(cos(4*glfwGetTime()))) );
			else
				mat4x4_scale3d(WMat, WMat, 0.9f);

			wID = glGetUniformLocation(context->shader_program, "W");
			glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);
			glDrawElements(GL_QUADS, 4 * context->dcube_mesh->nb_faces, GL_UNSIGNED_BYTE, context->dcube_mesh->indices);
		}


		glfwSwapBuffers (context->window);
	}

	logWrite ("[RENDR] Renderer stopped\n");

	return NULL;
};
