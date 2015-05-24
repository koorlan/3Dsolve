#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
	#include <windows.h>
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // Other kinds of Mac OS
		#endif
#elif __linux
    #include <unistd.h>
#else
		#error "Unspported Plateform"
#endif

#include <time.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"
#include "fonts.h"
#include "menu.h"

int main ( int argc, char ** argv )
{

	//0;1;2;1;2;1;2;1;2;2;2;2;1;2;1;2;2;2;1;2;2;1;2;2;2;1;0;
	if(!logStart())
	{
		logError ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}

	char* filePath = malloc(50);
	if(argc == 2)
		strncpy(filePath, argv[1], 50);
	else
		strncpy(filePath, "Snakes/snake.snake", 50);

	Snake* snake = snakeInit(filePath);
	free(filePath);

	if(snake == NULL)
		return -10;


	Context* context = contextCreate ();
	if ( !context )
	{
		logError ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}

	//Menu test;
	int i ;
	mymenu = NULL;
	initMenu(&mymenu);
	setMenuMargin(mymenu,(float []) {200.f, 50.f, 0.f, 0.f} );
	mymenu->margin[0] = 50.f;
	mymenu->margin[1] = 50.f;
	mymenu->margin[2] = 0.f;
	mymenu->margin[3] = 0.f;

	Item *tmpitem;
	for ( i = 0;  i < 10; i++) {
		initItem(&(tmpitem));
		setItemType(tmpitem, ITEM);
		//setItemStartCoord(tmpitem,(float[2]){-1.f+0.2f*i,1.f-0.2f*i});
		setItemMargin(tmpitem,(float[]){5.f,20.f,0.f,0.f,0.f});
		setItemDescriptor(tmpitem,(struct Descriptor)
			{	.name="Default Text",
				.font=LoadFont("fonts/Libertine.ttf"),
				.fontSize = 20,
				.color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
				.action= OPEN});
		addItemToMenu(mymenu,tmpitem);
	}

	setItemDescriptor(mymenu->item[3],(struct Descriptor)
		{	.name="Custom Text",
			.font=LoadFont("fonts/Quaaludes-Regular.ttf"),
			.fontSize = 55,
			.color=(struct Color){.r=1.0f,.g=0.0f,.b=0.0f,.a=1.0f},
			.action= OPEN});


	calcMenu(mymenu);

	logError ("MENU BBOX %f %f %f %f \n", mymenu->bbox[0], mymenu->bbox[1], mymenu->bbox[2], mymenu->bbox[3]);

	//End test menu


	context->snake = snake;

	resolverSolveSnake(snake);

//	snakePrintSolutions(snake);

	/*char *buffer = malloc(5*sizeof(char));

	if (snake->solutions->head!=NULL)
	{	ListSolution *tmpSol = malloc(sizeof(ListSolution));
		tmpSol = snake->solutions;
		{
			i=0;
			while(i < snake->length)
			{
				switch (tmpSol->head->step[i].dir)
				{
					case UP:
						strcpy(buffer,"UP");
						break;
					case DOWN:
						strcpy(buffer,"DOWN");
						break;
					case LEFT:
						strcpy(buffer,"LEFT");
						break;
					case RIGHT:
						strcpy(buffer,"RIGHT");
						break;
					case FRONT:
						strcpy(buffer,"FRONT");
						break;
					case BACK:
						strcpy(buffer,"BACK");
						break;
					case DNONE:
					default :
						printf("Error in solutions string\n");
						exit(-1);

				}
				i++;
			}
		}
	}*/

	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;

	contextInit ( context );

	while (context->running)
	{
		getInput(context);

		nanosleep(&time1, &time2);
	}

	contextDestroy ( context );

	snakeDestroy ( snake );

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
