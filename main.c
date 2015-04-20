#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"

int main ( int arg, char ** argv )
{

	if(!logStart())
	{
		logError ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}


	Snake* snake = snakeInit("Snakes/snake.snake");

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
	printf("Snakeresolved with %d  solutions \n",snake->solutions->size);
	char *buffer = malloc(5*sizeof(char));
	int i;
	for (i = 0;  i< snake->length; i++)
	{

		switch (snake->solutions->head->step[i].dir){
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
		}
		//printf("ste %d => %s à %d - %d - %d \n",i, buffer,snake->solutions->head->step[i].coord.x,snake->solutions->head->step[i].coord.y,snake->solutions->head->step[i].coord.z );
	}

	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;

	contextInit ( context );

	while (context->running)
	{
		int key = getInput(context);

		nanosleep(&time1, &time2);
	}

	contextDestroy ( context );

	snakeDestroy ( snake );

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
