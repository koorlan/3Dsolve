#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "context.h"
#include "log.h"

int main ( int arg, char ** argv )
{

	if(!log_start())
	{
		log_error ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}

	Context* context = context_create ();
	if ( !context )
	{
		log_error ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}
	context_init ( context );

	while ( context->running );

	context_destroy ( context );
	
	log_write ("[MAIN.] Terminated\n");

	return EXIT_SUCCESS;
}
