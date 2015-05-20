#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"

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
	context->snake = snake;

	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;

	contextInit ( context );

	resolverSolveSnake(snake);

	while (context->running)
	{
		getInput(context);

		nanosleep(&time1, &time2);
	}



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


	contextDestroy ( context );

	snakeDestroy ( snake );

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
