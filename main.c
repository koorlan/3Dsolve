#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"

int main ( int arg, char ** argv )
{

	if(!log_start())
	{
		log_error ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}


	Snake* snake = snakeInit("Snakes/snake.snake");

	if(snake == NULL)
		return;


	Context* context = context_create ();
	if ( !context )
	{
		log_error ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}
	context->snake = snake;

	context_init ( context );

	int i,j,k;
	/*
	for ( i = 0; i < snake->length; i++) {
		snake->tmpSteps[i].coord.x = 0;
		snake->tmpSteps[i].coord.y = 0;
		snake->tmpSteps[i].coord.z = i;
	}*/

	//Initialize a 3*3*3 Cube to fill
/*	for (i = 0; i < snake->volume.max.x; i++)
	{
		for (j = 0; j < snake->volume.max.y; j++)
		{
			for (k = 0; k < snake->volume.max.z; k++)
			{
				snake->volume.state[i][j][k] = FREE;
			}
		}
	}*/
	//With this snake

/*	snake->units[0] = EDGE ;
	snake->units[1] = STRAIGHT ;
	snake->units[2] = CORNER ;
	snake->units[3] = CORNER ;
	snake->units[4] = CORNER ;
	snake->units[5] = STRAIGHT ;
	snake->units[6] = CORNER ;
	snake->units[7] = CORNER ;
	snake->units[8] = STRAIGHT ;
	snake->units[9] = CORNER ;
	snake->units[10] = CORNER ;
	snake->units[11] = CORNER ;
	snake->units[12] = STRAIGHT ;
	snake->units[13] = CORNER ;
	snake->units[14] = STRAIGHT ;
	snake->units[15] = CORNER ;
	snake->units[16] = CORNER ;
	snake->units[17] = CORNER ;
	snake->units[18] = CORNER ;
	snake->units[19] = STRAIGHT ;
	snake->units[20] = CORNER ;
	snake->units[21] = STRAIGHT ;
	snake->units[22] = CORNER ;
	snake->units[23] = STRAIGHT ;
	snake->units[24] = CORNER ;
	snake->units[25] = STRAIGHT ;
	snake->units[26] = EDGE ;*/

	resolverSolveSnake(snake);
	printf("Snakeresolved with %d  solutions \n",snake->solutions->size);
	char *buffer = malloc(5*sizeof(char));
	for (i = 0;  i< snake->length; i++) {

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
