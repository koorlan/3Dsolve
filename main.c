#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "context.h"
#include "log.h"
#include "snake.h"


int main ( int arg, char ** argv )
{

	if(!log_start())
	{
		log_error ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}

	
	Snake* snake = snakeInit ();


	Context* context = context_create ();
	if ( !context )
	{
		log_error ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}
	context->snake = snake;

	context_init ( context );

	context->snake->tmpSteps[0].coord.x = 0;
	context->snake->tmpSteps[0].coord.y = 0;
	context->snake->tmpSteps[0].coord.z = 0;
	context->snake->tmpSteps[1].coord.x = 1;
	context->snake->tmpSteps[1].coord.y = 0;
	context->snake->tmpSteps[1].coord.z = 0;
	context->snake->tmpSteps[2].coord.x = 2;
	context->snake->tmpSteps[2].coord.y = 0;
	context->snake->tmpSteps[2].coord.z = 0;
	context->snake->tmpSteps[3].coord.x = 2;
	context->snake->tmpSteps[3].coord.y = 0;
	context->snake->tmpSteps[3].coord.z = -1;
	context->snake->currentUnit = 4;

	while ( context->running )
	{
		int key;
		key = getInput ( context );
		//printf("%d, %d\n", keys, key);
		if (key == 1 && snake->currentUnit < snake->length)
		{
			snake->tmpSteps[snake->currentUnit].coord.x = 2;
			snake->tmpSteps[snake->currentUnit].coord.y = snake->currentUnit-3;
			snake->tmpSteps[snake->currentUnit].coord.z = -1;
			snake->currentUnit++;
			sleep(1);
		}
	}

	context_destroy ( context );
	
	snakeDestroy ( snake );

	log_write ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
