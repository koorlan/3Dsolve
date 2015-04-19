#include "renderer.h"

void* renderer ( void *arg )
{

	log_write ("[RENDR] Renderer started\n");

	Context* context = (Context*) arg;
	
	context->running = 1;

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

		mat4x4_identity(WMat);
		mat4x4_rotate_X(WMat, WMat, M_PI * sin(glfwGetTime()));
		mat4x4_rotate_Y(WMat, WMat, glfwGetTime());		
		mat4x4_rotate_Z(WMat, WMat, M_PI * cos(glfwGetTime()));
		mat4x4_scale3d(WMat, WMat, (abs(cos(glfwGetTime()))+0.5f) );
		wID = glGetUniformLocation(context->shader_program, "W");
		glUniformMatrix4fv(wID, 1, GL_FALSE, &WMat[0][0]);

		vpID = glGetUniformLocation(context->shader_program, "VP");
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &PVMat[0][0]);

		//draw
		glClearColor( 0.25f, 0.25f, 0.25f, 1.f );
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_QUADS, 4 * context->cube_mesh->nb_faces, GL_UNSIGNED_BYTE, context->cube_mesh->indices);

		glfwSwapBuffers (context->window);
		glfwPollEvents ();
	}

	log_write ("[RENDR] Renderer stopped\n");

	return NULL;
};
