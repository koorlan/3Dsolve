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

	
	Snake* snek = snakeInit ();


	Context* context = context_create ();
	if ( !context )
	{
		log_error ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}
	context->snake = snek;
	context_init ( context );

	while ( context->running )
	{
		int key;
		key = getInput ();
		//printf("%d, %d\n", keys, key);
		if (key == 1 && snek->currentUnit < snek->length)
		{
			snek->tmpSteps[snek->currentUnit].coord.x = 0;
			snek->tmpSteps[snek->currentUnit].coord.y = snek->currentUnit;
			snek->tmpSteps[snek->currentUnit].coord.z = 0;
			snek->currentUnit++;
		}
	}

	context_destroy ( context );
	
	snakeDestroy ( snek );

	log_write ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
