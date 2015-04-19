#include "context.h"


void resizeCallback (GLFWwindow* window, int width, int height)
{
}

void buttonCallback(GLFWwindow* window, int button, int action, int modes)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		acc_x = xpos - last_xpos;
		acc_y = ypos - last_ypos;
	}
	
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	acc_x = xpos - last_xpos;
	acc_y = ypos - last_ypos;
	

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	switch(action)
	{
		case GLFW_PRESS:
			switch(key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
				case GLFW_KEY_UP:
					keys = 1;
					break;
			}
		break;
		case GLFW_RELEASE:
			switch(key)
			{
				case GLFW_KEY_UP:
					keys = 0;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

int getInput ()
{
	return keys;
}

Context* context_create ()
{

	Context* context = (Context*) malloc ( sizeof (Context) );

	if (!glfwInit ())
	{
		log_error ("[CNTXT] Could not start GLFW\n");
		return NULL;
	}
	log_write ("[CNTXT] GLFW started\n");

	context->window = NULL;
	if(FULLSCREEN)
	{
		GLFWmonitor* mon = glfwGetPrimaryMonitor ();
		const GLFWvidmode* vmode = glfwGetVideoMode ( mon );
		context->window = glfwCreateWindow ( vmode->width, vmode->height, "Solid Snake", mon, NULL );
		context->screen_width = vmode->width;
		context->screen_height = vmode->height;
	}
	else
	{
		context->screen_width = DRESX;
		context->screen_height = DRESY;
		context->window = glfwCreateWindow ( context->screen_width, context->screen_height, "Solid Snake", NULL, NULL );
	}
	log_write ("[CNTXT] Using resolution %dx%d\n", context->screen_width, context->screen_height);

	if ( !context->window ) {
		log_error ("[CNTXT] GLFW could not create window\n");
		glfwTerminate();
		context_destroy ( context );
		return NULL;
	}

	glfwMakeContextCurrent ( context->window );
	log_write ("[CNTXT] Window created\n");

	glfwSetKeyCallback(context->window, keyCallback);	
	glfwSetWindowSizeCallback(context->window, resizeCallback);	
	glfwSetCursorPosCallback(context->window, cursorCallback);
	glfwSetMouseButtonCallback (context->window, buttonCallback)

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_SAMPLES, 4);
	
	glewExperimental = GL_TRUE;
	glewInit ();
	log_write ("[CNTXT] GLEW started\n");

	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	log_write ("[CNTXT] Renderer: %s\n", renderer);
	log_write ("[CNTXT] OpenGL version supported %s\n", version);
	log_gl_params ();

	log_write ("[CNTXT] Context succesfully created\n");

	return context;

}

void context_init ( Context* context )
{
	log_write ("[CNTXT] Starting Renderer thread\n");

	GLuint vs = shader_load ("shaders/test_vs.glsl", GL_VERTEX_SHADER);
	GLuint fs = shader_load ("shaders/test_fs.glsl", GL_FRAGMENT_SHADER);
	shader_compile(vs);
	shader_compile(fs);
	context->shader_program = shader_create_program(vs, fs);
	context->cube_mesh = object_load ( "stc/cube.stc" );
	
	Camera * camera = camera_create();
	camera->eye[0] = 2.f;
	camera->eye[1] = 2.f;
	camera->eye[2] = 2.f;
	camera->target[0] = 0.f;
	camera->target[1] = 0.f;
	camera->target[2] = 0.f;
	camera->up[0] = 0.f;
	camera->up[1] = 1.f;
	camera->up[2] = 0.f;
	camera->angle[0] = 0.f;
	camera->angle[1] = 0.f;
	camera->fov = 1.3f;
	context->camera = camera;
	context->running = 1;
	pthread_create ( &context->render_thread, NULL, renderer, (void*)context );
}

void context_destroy ( Context * context )
{
	context->running = 0;
	pthread_join ( context->render_thread, NULL );
	glfwTerminate ();
	log_write ("[CNTXT] GLFW terminated\n");
	free ( context );
	log_write ("[CNTXT] Context destroyed\n");
}
