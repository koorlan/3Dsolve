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
			key_flags |= K_ANY;
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
					key_flags |= K_ENTER;
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

	if ( app->state == AS_HOME && key_flags!=0)
	{
		key_flags = K_NONE;
		app->state = AS_GAME;
	}
	else if ( (key_flags&K_ENTER)==K_ENTER )
	{
		bhv_flags ^= BHV_SPREAD;
	}
	else if ((key_flags&K_UP)==K_UP && context->playmode == PM_RESOLVE)
	{
		gsolver->selected = 0;
		gsolver->steps[0].dir = RIGHT;
		playerFlatten (gsolver, app->snake, 0);
		int i;
		for (i=0;i<app->snake->length;i++)
		{
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}

	}
	else if ((key_flags&K_UP)==K_UP && context->playmode == PM_PLAY)
	{
		gplayer->selected = 0;
		gplayer->steps[0].dir = RIGHT;
		playerFlatten (gplayer, app->snake, 0);
		int i;
		for (i=0;i<app->snake->length;i++)
		{
			mat4x4_translate(gplayer->realCubePos[i],
				(float) gplayer->steps[i].coord.x,
				(float) gplayer->steps[i].coord.y,
				(float) gplayer->steps[i].coord.z);
		}

	}
	else if ((key_flags&K_PGUP)==K_PGUP )
	{
		if ( app->snake->solutions != NULL && app->snake->solutions->head != NULL )
		{
			if (gsolver->currentSolution->next!=NULL) gsolver->currentSolution = gsolver->currentSolution->next;
			else gsolver->currentSolution = app->snake->solutions->head;
			gsolver->selected = 0;
			gsolver->steps[0].dir = RIGHT;
			playerFlatten (gsolver, app->snake, 0);
			int i;
			for (i=0; i<app->snake->length;i++)
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}
	}
	else if ((key_flags&K_DN)==K_DN)
	{
		context->playmode = ( context->playmode == PM_PLAY ? PM_RESOLVE : PM_PLAY );
		if (context->playmode == PM_RESOLVE)
		{
			gsolver->currentSolution = app->snake->solutions->head;
			playerFlatten(gplayer, app->snake, 0 );
		}
		else
		{
			int i;
			for (i=0; i<app->snake->length;i++)
			{
				gplayer->steps[i].dir = gsolver->steps[i].dir;
				gplayer->steps[i].coord.x = gsolver->steps[i].coord.x;
				gplayer->steps[i].coord.y = gsolver->steps[i].coord.y;
				gplayer->steps[i].coord.z = gsolver->steps[i].coord.z;
				mat4x4_dup (gplayer->realCubePos[i], gsolver->realCubePos[i]);
				mat4x4_dup (gplayer->realCubeRot[i], gsolver->realCubeRot[i]);
				gplayer->selected = gsolver->selected;
			}
		}
	}
	else if ((key_flags&K_LF)==K_LF && gsolver->currentSolution != NULL && context->playmode == PM_RESOLVE)
	{
		int i;

		for ( i=gsolver->selected-1; i < app->snake->length; i++ )
		{
			if ( gsolver->selected > 0) gsolver->selected--;
			if ( app->snake->units[gsolver->selected] == CORNER || app->snake->units[gsolver->selected] == EDGE )
				break;
		}

		Dir curDir = (gsolver->selected == 0 ? RIGHT : DNONE);//DNONE;;
		Dir prevDir = (gsolver->selected == 0 ? BACK : DNONE);
		int toggle = 0;
		for (i=0;i<=gsolver->selected;i++)
		{
			gsolver->steps[i].dir = gsolver->currentSolution->step[i].dir;
			if (gsolver->selected!=0)
			{
				prevDir = ( curDir != prevDir ? curDir : prevDir );
				curDir = gsolver->steps[i].dir;
			}
			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}
		for (i=gsolver->selected+1;i<app->snake->length;i++)
		{
			if (app->snake->units[i] == CORNER)
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
			else
			{
				if (toggle == 0)
					gsolver->steps[i].dir = curDir;
				else
					gsolver->steps[i].dir = prevDir;
			}


			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}


	}
	else if ((key_flags&K_LF)==K_LF && context->playmode == PM_PLAY)
	{
		int i;

		for ( i=gplayer->selected-1; i < app->snake->length; i++ )
		{
			if ( gplayer->selected > 0) gplayer->selected--;
			if ( app->snake->units[gplayer->selected] == CORNER || app->snake->units[gplayer->selected] == EDGE )
				break;
		}
		if (gplayer->selected < 1)
		{
			playerFlatten(gplayer, app->snake, 0 );
			for (i=0;i<app->snake->length;i++)
			{
				mat4x4_translate(gplayer->realCubePos[i],
					(float) gplayer->steps[i].coord.x,
					(float) gplayer->steps[i].coord.y,
					(float) gplayer->steps[i].coord.z);
			}
		}
		else
		{
			Dir curDir = (gplayer->selected == 0 ? RIGHT : DNONE);//DNONE;;
			Dir prevDir = DNONE;
			int toggle = 0;
			for (i=0;i<=gplayer->selected;i++)
			{
				prevDir = ( curDir != prevDir ? curDir : prevDir );
				curDir = gplayer->steps[i].dir;
				gplayer->steps[i].coord.x = (i==0?0:gplayer->steps[i-1].coord.x+dir2int[gplayer->steps[i-1].dir][0]);
				gplayer->steps[i].coord.y = (i==0?0:gplayer->steps[i-1].coord.y+dir2int[gplayer->steps[i-1].dir][1]);
				gplayer->steps[i].coord.z = (i==0?0:gplayer->steps[i-1].coord.z+dir2int[gplayer->steps[i-1].dir][2]);
				mat4x4_translate(gplayer->realCubePos[i],
					(float) gplayer->steps[i].coord.x,
					(float) gplayer->steps[i].coord.y,
					(float) gplayer->steps[i].coord.z);
			}
			for (i=gplayer->selected+1;i<app->snake->length;i++)
			{
				if (app->snake->units[i] == CORNER)
				{
					if (toggle == 0)
					{
						gplayer->steps[i].dir = prevDir;
						toggle = 1;
					}
					else
					{
						gplayer->steps[i].dir = curDir;
						toggle = 0;
					}
				}
				else
				{
					if (toggle == 0)
						gplayer->steps[i].dir = curDir;
					else
						gplayer->steps[i].dir = prevDir;
				}


				gplayer->steps[i].coord.x = (i==0?0:gplayer->steps[i-1].coord.x+dir2int[gplayer->steps[i-1].dir][0]);
				gplayer->steps[i].coord.y = (i==0?0:gplayer->steps[i-1].coord.y+dir2int[gplayer->steps[i-1].dir][1]);
				gplayer->steps[i].coord.z = (i==0?0:gplayer->steps[i-1].coord.z+dir2int[gplayer->steps[i-1].dir][2]);
				mat4x4_translate(gplayer->realCubePos[i],
					(float) gplayer->steps[i].coord.x,
					(float) gplayer->steps[i].coord.y,
					(float) gplayer->steps[i].coord.z);
			}
		}
	}
	else if ((key_flags&K_RT)==K_RT&& gsolver->currentSolution != NULL && context->playmode == PM_RESOLVE)
	{
		int i;

		gsolver->steps[0].dir = app->snake->solutions->head->step[0].dir;

		for ( i=gsolver->selected+1; i < app->snake->length; i++ )
		{
			if ( gsolver->selected < app->snake->length + 1) gsolver->selected++;
			if ( app->snake->units[gsolver->selected] == CORNER || app->snake->units[gsolver->selected] == EDGE )
				break;
		}

		Dir curDir = (gsolver->selected == 0 ? RIGHT : DNONE);//DNONE;
		Dir prevDir = (gsolver->selected == 0 ? BACK : DNONE);
		int toggle = 0;
		for (i=0;i<=gsolver->selected;i++)
		{
			gsolver->steps[i].dir = gsolver->currentSolution->step[i].dir;
			if (gsolver->selected!=0)
			{
				prevDir = ( curDir != prevDir ? curDir : prevDir );
				curDir = gsolver->steps[i].dir;
			}
			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}
		for (i=gsolver->selected+1;i<app->snake->length;i++)
		{
			if (app->snake->units[i] == CORNER)
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
			else
			{
				if (toggle == 0)
					gsolver->steps[i].dir = curDir;
				else
					gsolver->steps[i].dir = prevDir;
			}

			gsolver->steps[i].coord.x = (i==0?0:gsolver->steps[i-1].coord.x+dir2int[gsolver->steps[i-1].dir][0]);
			gsolver->steps[i].coord.y = (i==0?0:gsolver->steps[i-1].coord.y+dir2int[gsolver->steps[i-1].dir][1]);
			gsolver->steps[i].coord.z = (i==0?0:gsolver->steps[i-1].coord.z+dir2int[gsolver->steps[i-1].dir][2]);
			mat4x4_translate(gsolver->realCubePos[i],
				(float) gsolver->steps[i].coord.x,
				(float) gsolver->steps[i].coord.y,
				(float) gsolver->steps[i].coord.z);
		}

	}
	else if ((key_flags&K_RT)==K_RT && context->playmode == PM_PLAY)
	{
		playerHelp(gplayer, app->snake);
		int i;
		for (i=0;i<=gplayer->selected;i++)
			mat4x4_translate(gplayer->realCubePos[i],
				(float) gplayer->steps[i].coord.x,
				(float) gplayer->steps[i].coord.y,
				(float) gplayer->steps[i].coord.z);

		Dir curDir = DNONE;
		Dir prevDir = DNONE;
		int toggle = 0;
		for (i=0;i<=gplayer->selected;i++)
		{
			prevDir = ( curDir != prevDir ? curDir : prevDir );
			curDir = gplayer->steps[i].dir;
		}
		for (i=gplayer->selected+1;i<app->snake->length;i++)
		{
			if (app->snake->units[i] == CORNER)
			{
				if (toggle == 0)
				{
					gplayer->steps[i].dir = prevDir;
					toggle = 1;
				}
				else
				{
					gplayer->steps[i].dir = curDir;
					toggle = 0;
				}
			}
			else
			{
				if (toggle == 0)
					gplayer->steps[i].dir = curDir;
				else
					gplayer->steps[i].dir = prevDir;
			}

			gplayer->steps[i].coord.x = (gplayer->steps[i-1].coord.x+dir2int[gplayer->steps[i-1].dir][0]);
			gplayer->steps[i].coord.y = (gplayer->steps[i-1].coord.y+dir2int[gplayer->steps[i-1].dir][1]);
			gplayer->steps[i].coord.z = (gplayer->steps[i-1].coord.z+dir2int[gplayer->steps[i-1].dir][2]);
			mat4x4_translate(gplayer->realCubePos[i],
				(float) gplayer->steps[i].coord.x,
				(float) gplayer->steps[i].coord.y,
				(float) gplayer->steps[i].coord.z);


		}

	}

	if ((mouse_flags&M_RLEFTONCE)==M_RLEFTONCE)
	{

		if( app->itemSelected >= 0)
		{
			int i = 0,
					j = 0,
					closedMenu = 0;
					Solution *newSolution;
			Menu *currentMenu = NULL;
			Menu *menuToClose = NULL;
			currentMenu = app->menu;
			int accumulator = 0;
			//From 279 to 307 , Magic...do not touch !
		for ( i = 0; i < app->menuDepth; i++) {
			accumulator += currentMenu->size;
			logWrite("accumulator %d \n",accumulator);
			if(app->itemSelected >= accumulator){
				if(currentMenu->opened >= 0 && currentMenu->opened < currentMenu->size)
					currentMenu = currentMenu->item[currentMenu->opened]->menu;
				else
					break;
			}
			else{
				logWrite("itemSelected : %d , acc %d, menusize %d \n",app->itemSelected , accumulator ,currentMenu->size);
				currentMenu->selected = app->itemSelected - accumulator + currentMenu->size;
				if (currentMenu->opened >= 0 && currentMenu->opened < currentMenu->size){
					menuToClose = currentMenu->item[currentMenu->opened]->menu;
				for ( j = i+1; j < app->menuDepth; j++) {
					if(currentMenu->selected == currentMenu->opened)
						break;
					menuToClose->state = CLOSE;
					closedMenu ++;
					if (menuToClose->opened >= 0 && menuToClose->opened < menuToClose->size){
						menuToClose = menuToClose->item[menuToClose->opened]->menu;
					}
				}
				app->menuDepth -= closedMenu;
				}
				break;
			}
		}
		if (currentMenu->selected > -1 && currentMenu->selected < currentMenu->size){
			switch (currentMenu->item[currentMenu->selected]->descriptor.action){
				case RESET:
					break;
				case EXIT:
					app->running = 0;
					break;
				case LOADSNAKE:
					app->state = AS_HOME;
					sleep(1);
					// Chargement d'un nouveau snake
					logWrite("[MENU] New snake requested\n");
					// Récupération du nom du snake
					char* snakeName = currentMenu->item[currentMenu->selected]->descriptor.name;
					char* snakePath = malloc((10 + strlen(snakeName)) * sizeof(char));
					sprintf(snakePath, "Snakes/%s", snakeName);
					logWrite("[MENU] Loading Snake : %s\n", snakePath);
					Snake* newSnake = snakeInit(snakePath);
					free(snakePath);
					currentMenu->state = CLOSE;
					app->menuDepth --;
					if(newSnake != NULL)
					{
						snakeDestroy(app->snake, 1);
						app->snake = newSnake;
						resolverSolveSnake(app->snake, NULL);

						playerDestroy(gplayer);
						gplayer = playerInit ( app->snake );
						gplayer->selected = 0;
						playerDestroy(gsolver);
						gsolver = playerInit(app->snake);
						gsolver->currentSolution = app->snake->solutions->head;
					}
					app->state = AS_GAME;
					break;
				case LOADSOL:
				newSolution = app->snake->solutions->head;
					for(i=0; i<currentMenu->selected;i++){
						if ( app->snake->solutions != NULL && app->snake->solutions->head != NULL )
						{
							if (newSolution->next!=NULL) newSolution = newSolution->next;
							else newSolution = app->snake->solutions->head;
						}
					}
						gsolver->currentSolution = newSolution;
						gsolver->selected = 0;
						gsolver->steps[0].dir = RIGHT;
						playerFlatten (gsolver, app->snake, 0);
						int i;
						for (i=0;i<app->snake->length;i++)
						{
							mat4x4_translate(gsolver->realCubePos[i],
								(float) gsolver->steps[i].coord.x,
								(float) gsolver->steps[i].coord.y,
								(float) gsolver->steps[i].coord.z);
						}
						context->playmode = PM_RESOLVE;
					break;
				case MENU:
					if(currentMenu->item[currentMenu->selected]->menu != NULL && currentMenu->item[currentMenu->selected]->menu->state == CLOSE){
						currentMenu->item[currentMenu->selected]->menu->state = OPEN;
						currentMenu->opened = currentMenu->selected;
						app->menuDepth ++;
						break;
					}
					if(currentMenu->item[currentMenu->selected]->menu != NULL && currentMenu->item[currentMenu->selected]->menu->state == OPEN){
						currentMenu->item[currentMenu->selected]->menu->state = CLOSE;
						app->menuDepth --;
						break;
					}
					break;
				default:
					break;
			}
			currentMenu->opened = currentMenu->selected;
			currentMenu->selected = -1;
		}
	}

		if (gplayer->selected!=0)
		{
			if(context->playmode == PM_RESOLVE){
				int i;
				for (i=0; i<app->snake->length;i++)
				{
					gplayer->steps[i].dir = gsolver->steps[i].dir;
					gplayer->steps[i].coord.x = gsolver->steps[i].coord.x;
					gplayer->steps[i].coord.y = gsolver->steps[i].coord.y;
					gplayer->steps[i].coord.z = gsolver->steps[i].coord.z;
					mat4x4_dup (gplayer->realCubePos[i], gsolver->realCubePos[i]);
					mat4x4_dup (gplayer->realCubeRot[i], gsolver->realCubeRot[i]);
					gplayer->selected = gsolver->selected;
				}
				context->playmode = PM_PLAY;
			}
			else{
				playerRotate(gplayer, gplayer->selected, app->snake, 0);
				playerRotate(gplayer, gplayer->selected, app->snake, 1);
			}

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
				playerRotate(gplayer, gplayer->selected, app->snake, magnet);

				magnet = 0;
			}
			else playerFakeRotate(gplayer, gplayer->selected, app->snake, magnet);
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
		context->window = glfwCreateWindow ( vmode->width, vmode->height, "3DSolve", mon, NULL );
		context->screen_width = vmode->width;
		context->screen_height = vmode->height;
	}
	else
	{
		context->screen_width = DRESX;
		context->screen_height = DRESY;
		context->window = glfwCreateWindow ( context->screen_width, context->screen_height, "3DSolve", NULL, NULL );
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

	context->snake_program_vs = shaderLoad ("shaders/default_vs.glsl", GL_VERTEX_SHADER);
	context->snake_program_fs = shaderLoad ("shaders/default_fs.glsl", GL_FRAGMENT_SHADER);
	shaderCompile(context->snake_program_vs);
	shaderCompile(context->snake_program_fs);
	context->snake_program = shaderCreateProgram(context->snake_program_vs, context->snake_program_fs);

	context->picking_program_vs = shaderLoad ("shaders/pick_vs.glsl", GL_VERTEX_SHADER);
	context->picking_program_fs = shaderLoad ("shaders/pick_fs.glsl", GL_FRAGMENT_SHADER);
	shaderCompile(context->picking_program_vs);
	shaderCompile(context->picking_program_fs);
	context->picking_program = shaderCreateProgram(context->picking_program_vs, context->picking_program_fs);

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
	free(buffer);

	lodepng_decode32_file(&buffer, &width, &height, "textures/map2.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->lwoodtex = textureID;
	free(buffer);

	lodepng_decode32_file(&buffer, &width, &height, "textures/map3.png");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	context->dwoodtex = textureID;
	free(buffer);


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
	vol_offset[0]=(app->snake->volume.max.x%2==0 ? app->snake->volume.max.x /2 - 0.5f : (app->snake->volume.max.x)/2);
	vol_offset[1]=(app->snake->volume.max.y%2==0 ? app->snake->volume.max.y /2 - 0.5f : (app->snake->volume.max.y)/2);
	vol_offset[2]=(app->snake->volume.max.z%2==0 ? app->snake->volume.max.z /2 - 0.5f : (app->snake->volume.max.z)/2);

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
	camera->angle[1] = 0.5f;
	camera->fov = 1.6f;
	camera->distance = 4.f;
	context->camera = camera;
	context->drawpick = 0;
	context->camera->eye[0] = context->camera->target[0] + context->camera->distance
				* sin(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[2] = context->camera->target[1] + context->camera->distance
				* cos(context->camera->angle[0]) * cos(context->camera->angle[1]);
	context->camera->eye[1] = context->camera->target[2] + context->camera->distance
				* sin(context->camera->angle[1]);

	glfwMakeContextCurrent ( NULL );

	pthread_create ( &context->render_thread, NULL, renderer, (void*)context );
}

void contextDestroy ( Context * context )
{
	objectDestroy (context->cube_mesh);
	objectDestroy (context->square_mesh);
	objectDestroy (context->link_mesh);
	shaderProgramDestroy(context->snake_program,context->snake_program_vs,context->snake_program_fs);
	shaderProgramDestroy(context->picking_program,context->picking_program_vs,context->picking_program_fs);

	pthread_join ( context->render_thread, NULL );
	logWrite ("[CNTXT] Graphic thread terminated\n");
	glfwMakeContextCurrent ( NULL );
	glfwTerminate ();
	logWrite ("[CNTXT] GLFW terminated\n");
	free ( context );
	logWrite ("[CNTXT] Context destroyed\n");
}
