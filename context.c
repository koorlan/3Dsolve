#include "context.h"

extern const int dir2int[6][3];

void resizeCallback (GLFWwindow* window, int width, int height)
{
	resize_w = width;
	resize_h = height;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset>0) mouse_flags |= M_ROLLF;
	else if (yoffset<0) mouse_flags |= M_ROLLB;
}

void buttonCallback(GLFWwindow* window, int button, int action, int modes)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		mouse_flags |= M_RIGHT;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		mouse_flags ^= M_RIGHT;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouse_flags |= M_LEFT;
		mouse_flags |= M_LEFTONCE;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mouse_flags ^= M_LEFT;
		mouse_flags |= M_RLEFTONCE;
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
				case GLFW_KEY_PAGE_UP:
					key_flags |= K_PGUP;
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
				case GLFW_KEY_ENTER:
					bhv_flags ^= BHV_SPREAD;
					break;
				case GLFW_KEY_SPACE:
					bhv_flags ^= BHV_ROTATE;
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
	static int magnet;

	//mouse_flags = M_NONE;
	last_xpos = gxpos;
	last_ypos = gypos;
	key_flags = M_NONE;
	resize_h = -1;
	resize_w = -1;
	glfwPollEvents ();

	if (resize_h!=-1 || resize_w!=-1)
	{

		context->screen_width = resize_w;
		context->screen_height = resize_h;
		context->ratio = ((float)resize_w)/(float)resize_h;

	//	testMenuMesh(app->menu, context->screen_width	, context->screen_height);

		resize_h = -1;
		resize_w = -1;

	}

	if ((key_flags&K_UP)==K_UP)
	{
		gplayer->selected = 0;
		playerFlatten ( gplayer, context->snake, 0 );
	}
	else if ((key_flags&K_PGUP)==K_PGUP)
	{
		if ( context->snake->solutions != NULL && context->snake->solutions->head != NULL )
		{
			if (gsolver->currentSolution->next!=NULL) gsolver->currentSolution = gsolver->currentSolution->next;
			else gsolver->currentSolution = context->snake->solutions->head;
			gsolver->selected = 0;
			gsolver->steps[0].dir = RIGHT;
			playerFlatten (gsolver, context->snake, 0);
		}
	}
	else if ((key_flags&K_DN)==K_DN)
	{
		context->playmode = ( context->playmode == PM_PLAY ? PM_RESOLVE : PM_PLAY );
		if (context->playmode == PM_RESOLVE)
		{
			gsolver->currentSolution = context->snake->solutions->head;
		}
	}
	else if ((key_flags&K_LF)==K_LF && gsolver->currentSolution != NULL)
	{
		int i;

		for ( i=gsolver->selected-1; i < context->snake->length; i++ )
		{
			if ( gsolver->selected > 0) gsolver->selected--;
			if ( context->snake->units[gsolver->selected] == CORNER || context->snake->units[gsolver->selected] == EDGE )
				break;
		}

		Dir curDir = (gsolver->selected == 0 ? RIGHT : DNONE);//DNONE;;
		Dir prevDir = DNONE;
		int toggle = 0;
		for (i=0;i<=gsolver->selected;i++)
		{
			gsolver->steps[i].dir = gsolver->currentSolution->step[i].dir;
			prevDir = ( curDir != prevDir ? curDir : prevDir );
			curDir = gsolver->steps[i].dir;
			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}
		for (i=gsolver->selected+1;i<context->snake->length;i++)
		{
			if (context->snake->units[i] == CORNER)
			{
				if (toggle == 0)
				{
					gsolver->steps[i].dir = prevDir;
					toggle = 1;
				}
				else
				{
					gsolver->steps[i].dir = curDir;
					toggle = 0;
				}
			}
			else gsolver->steps[i].dir = curDir;

			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}


	}
	else if ((key_flags&K_RT)==K_RT && gsolver->currentSolution != NULL)
	{
		int i;

		gsolver->steps[0].dir = context->snake->solutions->head->step[0].dir;

		for ( i=gsolver->selected+1; i < context->snake->length; i++ )
		{
			if ( gsolver->selected < context->snake->length + 1) gsolver->selected++;
			if ( context->snake->units[gsolver->selected] == CORNER || context->snake->units[gsolver->selected] == EDGE )
				break;
		}

		Dir curDir = (gsolver->selected == 0 ? RIGHT : DNONE);//DNONE;
		Dir prevDir = DNONE;//(gsolver->selected == 0 ? RIGHT : DNONE);
		int toggle = 0;
		for (i=0;i<=gsolver->selected;i++)
		{
			gsolver->steps[i].dir = gsolver->currentSolution->step[i].dir;
			prevDir = ( curDir != prevDir ? curDir : prevDir );
			curDir = gsolver->steps[i].dir;
			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}
		for (i=gsolver->selected+1;i<context->snake->length;i++)
		{
			if (context->snake->units[i] == CORNER)
			{
				if (toggle == 0)
				{
					gsolver->steps[i].dir = prevDir;
					toggle = 1;
				}
				else
				{
					gsolver->steps[i].dir = curDir;
					toggle = 0;
				}
			}
			else gsolver->steps[i].dir = curDir;

			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}

	}

	if ((mouse_flags&M_RLEFTONCE)==M_RLEFTONCE)
	{

		if (app->menu->selected != -1 && app->menu->selected < app->menu->size){
			switch (app->menu->item[app->menu->selected]->descriptor.action){
				case RESET:
					logWrite("[MENU] Close Trigger (item %d)\n",app->menu->selected);
					printf("[MENU] Close Trigger (item %d)\n",app->menu->selected );
					break;
				case TEST:
					logWrite("[MENU] Test Trigger (item %d)\n",app->menu->selected);
					printf("[MENU] Close Trigger (item %d)\n",app->menu->selected );
					break;
				case MENU:
					logWrite("[MENU] Open Trigger (item %d)\n",app->menu->selected);
					if(app->menu->item[app->menu->selected]->menu != NULL && app->menu->item[app->menu->selected]->menu->state == CLOSE){
						app->menu->item[app->menu->selected]->menu->state = OPEN;
						app->menuDepth ++;
						break;
					}
					if(app->menu->item[app->menu->selected]->menu != NULL && app->menu->item[app->menu->selected]->menu->state == OPEN){
						app->menu->item[app->menu->selected]->menu->state = CLOSE;
						app->menuDepth --;
						break;
					}
					break;
				default:
					break;
			}
			app->menu->selected = -1;
		}


		if (gplayer->selected!=0)
		{
			playerRotate(gplayer, gplayer->selected, context->snake, 0);
			playerRotate(gplayer, gplayer->selected, context->snake, 1);
		}

		magnet = 0;
		mouse_flags ^= M_RLEFTONCE;
	}

	if ((mouse_flags&M_LEFT)==M_LEFT)
	{
		if ((mouse_flags&M_LEFTONCE)==M_LEFTONCE)
		{
			context->drawpick = 1;
				mouse_flags ^= M_LEFTONCE;
		}
		float accx = (last_xpos-gxpos)*0.01f;
		float accy = (last_ypos-gypos)*0.01f;
		if ( ( accx!=0.f || accy!=0.f ) && gplayer->selected != -1 )
		{
			//TODO: meilleure detection du sens de rotation
			//if	(accx>0 && accy>0) magnet += MAG_STEP;
			//else if (accx<0) magnet -= MAG_STEP;
			if ( abs(accx) > abs(accy) )
			{
				if (accx<0) magnet += MAG_STEP;
				else if (accx>0) magnet -= MAG_STEP;
			}
			else
			{
				if (accy>0) magnet += MAG_STEP;
				else if (accy<0) magnet -= MAG_STEP;
			}

			if ( magnet > MAG_TRESHOLD || magnet < -MAG_TRESHOLD)
			{
				playerRotate(gplayer, gplayer->selected, context->snake, magnet);
				magnet = 0;
			}
			else playerFakeRotate(gplayer, gplayer->selected, context->snake, magnet);
		}
	}


	if ((mouse_flags&M_RIGHT)==M_RIGHT)
	{
		float accx = (last_xpos-gxpos)*0.01f;
		float accy = (last_ypos-gypos)*0.01f;
		context->camera->angle[0] += accx;
		context->camera->angle[1] += accy;
		if (context->camera->angle[1] > (M_PI/2-0.01f)) context->camera->angle[1] = (M_PI/2-0.01f);
		if (context->camera->angle[1] < (-M_PI/2+0.01f)) context->camera->angle[1] = (-M_PI/2+0.01f);
	}
	else if ((bhv_flags&BHV_ROTATE)==BHV_ROTATE)
	{
		context->camera->angle[0]+=0.002f;
	}

	if ((bhv_flags&BHV_SPREAD)==BHV_SPREAD)
		context->spread = 1;
	else context->spread = 0;

	if ((mouse_flags&M_ROLLF)==M_ROLLF)
	{
		context->camera->distance-=0.4f;
		mouse_flags = M_NONE;
	}
	else if ((mouse_flags&M_ROLLB)==M_ROLLB)
	{
		context->camera->distance+=0.4f;
		mouse_flags = M_NONE;
	}

	//mise à jour de la position/orientation de la camera
	context->camera->eye[0] = context->camera->target[0] + context->camera->distance
				* sin(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[2] = context->camera->target[1] + context->camera->distance
				* cos(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[1] = context->camera->target[2] + context->camera->distance
				* sin(context->camera->angle[1]);

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

	glfwWindowHint (GLFW_SAMPLES, 4);

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
	glfwSetScrollCallback (context->window, scrollCallback);

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(1);

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation (GL_FUNC_ADD);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	GLuint vs = shaderLoad ("shaders/default_vs.glsl", GL_VERTEX_SHADER);
	GLuint fs = shaderLoad ("shaders/default_fs.glsl", GL_FRAGMENT_SHADER);
	shaderCompile(vs);
	shaderCompile(fs);
	context->snake_program = shaderCreateProgram(vs, fs);

	vs = shaderLoad ("shaders/pick_vs.glsl", GL_VERTEX_SHADER);
	fs = shaderLoad ("shaders/pick_fs.glsl", GL_FRAGMENT_SHADER);
	shaderCompile(vs);
	shaderCompile(fs);
	context->picking_program = shaderCreateProgram(vs, fs);

	context->cube_mesh = objectLoad ( "stc/woodcube4.obj" );
	context->square_mesh = objectLoad ( "stc/square.stc" );
	context->link_mesh = objectLoad ( "stc/link.stc" );

	unsigned char* buffer;
	unsigned int width, height;
	GLuint textureID;

	lodepng_decode32_file(&buffer, &width, &height, "textures/rope.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->linktex = textureID;

	lodepng_decode32_file(&buffer, &width, &height, "textures/map2.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->lwoodtex = textureID;

	lodepng_decode32_file(&buffer, &width, &height, "textures/map3.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->dwoodtex = textureID;


	lodepng_decode32_file(&buffer, &width, &height, "textures/menu.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->menutex = textureID;

	lodepng_decode32_file(&buffer, &width, &height, "textures/item.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->itemtex = textureID;

	vec3 vol_offset;
	vol_offset[0]=(context->snake->volume.max.x%2==0 ? context->snake->volume.max.x /2 - 0.5f : (context->snake->volume.max.x -1 )/2);
	vol_offset[1]=(context->snake->volume.max.y%2==0 ? context->snake->volume.max.y /2 - 0.5f : (context->snake->volume.max.y -1 )/2);
	vol_offset[2]=(context->snake->volume.max.z%2==0 ? context->snake->volume.max.z /2 - 0.5f : (context->snake->volume.max.z -1 )/2);

	Camera * camera = cameraCreate();
	camera->eye[0] = 0.f;
	camera->eye[1] = 0.f;
	camera->eye[2] = 0.f;
	camera->target[0] = vol_offset[0];
	camera->target[1] = vol_offset[1];
	camera->target[2] = vol_offset[2];
	camera->up[0] = 0.f;
	camera->up[1] = 1.f;
	camera->up[2] = 0.f;
	camera->angle[0] = 0.f;
	camera->angle[1] = 0.8f;
	camera->fov = 1.6f;
	camera->distance = 4.f;
	context->camera = camera;

	context->drawpick = 0;
	//bhv_flags |= BHV_ROTATE;

	context->running = 1;
	context->playmode = PM_PLAY;

	gplayer = playerInit ( context->snake );
	gsolver = playerInit ( context->snake );
	gsolver->currentSolution = context->snake->solutions->head;

	glfwMakeContextCurrent ( NULL );

	pthread_create ( &context->render_thread, NULL, renderer, (void*)context );
}

void contextDestroy ( Context * context )
{
	context->running = 0;
	pthread_join ( context->render_thread, NULL );
	logWrite ("[CNTXT] Graphic thread terminated\n");
	glfwMakeContextCurrent ( NULL );
	glfwTerminate ();
	logWrite ("[CNTXT] GLFW terminated\n");
	free ( context );
	logWrite ("[CNTXT] Context destroyed\n");
}
