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

int main ( int argc, char ** argv )
{
	myfont = LoadFont("fonts/Libertine.ttf");

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
