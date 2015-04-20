#include "context.h"


void resizeCallback (GLFWwindow* window, int width, int height)
{
}

void buttonCallback(GLFWwindow* window, int button, int action, int modes)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouse_flags |= M_RIGHT;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouse_flags ^= M_RIGHT;
	}
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_flags |= M_MOVE;
	gxpos = xpos;
	gypos = ypos;
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
					key_flags |= K_UP;
					break;
				case GLFW_KEY_DOWN:
					key_flags |= K_DN;
					break;
				case GLFW_KEY_LEFT:
					key_flags |= K_LF;
					break;
				case GLFW_KEY_RIGHT:
					key_flags |= K_RT;
					break;
			}
		break;
		case GLFW_RELEASE:
			switch(key)
			{
				default:
					break;
			}
			break;
		default:
			break;
	}
}

int getInput ( Context* context )
{
	//mouse_flags = M_NONE;
	last_xpos = gxpos;
	last_ypos = gypos;
	key_flags = M_NONE;
	glfwPollEvents ();
	
	if ((key_flags&K_UP)==K_UP)
	{
	}
	else if ((key_flags&K_DN)==K_DN)
	{
	}
	else if ((key_flags&K_LF)==K_LF)
	{
		context->snake->currentUnit =
			(context->snake->currentUnit>0?context->snake->currentUnit-1:0);
	}
	else if ((key_flags&K_RT)==K_RT)
	{
		context->snake->currentUnit =
			(context->snake->currentUnit>=context->snake->length-2?context->snake->length-1:context->snake->currentUnit+1);
	}

	if ((mouse_flags&M_RIGHT)==M_RIGHT)
	{
		float accx = (last_xpos-gxpos)*0.01f;
		float accy = (last_ypos-gypos)*0.01f;
		context->camera->angle[0] += accx;
		context->camera->angle[1] += accy;
		if (context->camera->angle[1] > (M_PI/2-0.01f)) context->camera->angle[1] = (M_PI/2-0.01f);
		if (context->camera->angle[1] < (-M_PI/2+0.01f)) context->camera->angle[1] = (-M_PI/2+0.01f);
		//printf("accx=%f  accy=%f\n", accx, accy);
	}
	else
	{
		context->camera->angle[0]+=0.001f;
	}

	context->camera->eye[0] = context->camera->distance * sin(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[2] = context->camera->distance * cos(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[1] = context->camera->distance * sin(context->camera->angle[1]);

	return 0;
}

Context* contextCreate ()
{

	Context* context = (Context*) malloc ( sizeof (Context) );

	if (!glfwInit ())
	{
		logError ("[CNTXT] Could not start GLFW\n");
		return NULL;
	}
	logWrite ("[CNTXT] GLFW started\n");

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
	logWrite ("[CNTXT] Using resolution %dx%d\n", context->screen_width, context->screen_height);

	if ( !context->window ) {
		logError ("[CNTXT] GLFW could not create window\n");
		glfwTerminate();
		contextDestroy ( context );
		return NULL;
	}

	glfwMakeContextCurrent ( context->window );
	logWrite ("[CNTXT] Window created\n");

	glfwSetKeyCallback(context->window, keyCallback);
	glfwSetWindowSizeCallback(context->window, resizeCallback);
	glfwSetCursorPosCallback(context->window, cursorCallback);
	glfwSetMouseButtonCallback (context->window, buttonCallback);

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_SAMPLES, 4);

	glewExperimental = GL_TRUE;
	glewInit ();
	logWrite ("[CNTXT] GLEW started\n");

	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	logWrite ("[CNTXT] Renderer: %s\n", renderer);
	logWrite ("[CNTXT] OpenGL version supported %s\n", version);
	logGLParams ();

	logWrite ("[CNTXT] Context succesfully created\n");

	return context;

}

void contextInit ( Context* context )
{
	logWrite ("[CNTXT] Starting Renderer thread\n");

	GLuint vs = shaderLoad ("shaders/test_vs.glsl", GL_VERTEX_SHADER);
	GLuint fs = shaderLoad ("shaders/test_fs.glsl", GL_FRAGMENT_SHADER);
	shaderCompile(vs);
	shaderCompile(fs);
	context->shader_program = shaderCreateProgram(vs, fs);
	context->dcube_mesh = objectLoad ( "stc/dcube.stc" );
	context->lcube_mesh = objectLoad ( "stc/lcube.stc" );

	Camera * camera = cameraCreate();
	camera->eye[0] = 7.f;
	camera->eye[1] = 0.f;
	camera->eye[2] = 0.f;
	camera->target[0] = 0.f;
	camera->target[1] = 0.f;
	camera->target[2] = 0.f;
	camera->up[0] = 0.f;
	camera->up[1] = 1.f;
	camera->up[2] = 0.f;
	camera->angle[0] = 0.f;
	camera->angle[1] = 0.5f;
	camera->fov = 1.6f;
	camera->distance = 4.f;
	context->camera = camera;

	context->running = 1;
	glfwMakeContextCurrent ( NULL );
	pthread_create ( &context->render_thread, NULL, renderer, (void*)context );
}

void contextDestroy ( Context * context )
{
	context->running = 0;
	pthread_join ( context->render_thread, NULL );
	glfwTerminate ();
	logWrite ("[CNTXT] GLFW terminated\n");
	free ( context );
	logWrite ("[CNTXT] Context destroyed\n");
}
