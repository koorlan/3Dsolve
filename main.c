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

	int i;
	for ( i = 0; i < snake->length; i++) {
		context->snake->tmpSteps[i].coord.x = i;
		context->snake->tmpSteps[i].coord.y = 0;
		context->snake->tmpSteps[i].coord.z = 0;
	}


	while ( context->running )
	{
		int key;
		key = getInput ( context );
		//printf("%d, %d\n", keys, key);
		if (key == 1 && snake->currentUnit < snake->length)
		{
			//snake->tmpSteps[snake->currentUnit].coord.x = 2;
			//snake->tmpSteps[snake->currentUnit].coord.y = snake->currentUnit-3;
			//snake->tmpSteps[snake->currentUnit].coord.z = -1;
			snake->currentUnit++;
			sleep(1);
		}
	}

	context_destroy ( context );

	snakeDestroy ( snake );

	log_write ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
