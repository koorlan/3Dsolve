#include "snake.h"

Snake* snakeInit ()
{
	Snake* snake = malloc ( sizeof(Snake) );
	snake->length = 27; //ne to be read from the template file
	snake->tmpSteps = malloc ( snake->length * sizeof(Step) );
	snake->units = malloc ( snake->length * sizeof(Unit) );
	snake->currentUnit = 0;
	snake->solutions = listSolutionCreate();
	snake->volume.max.x = 3;
	snake->volume.max.y = 3;
	snake->volume.max.z = 3;
//	snake->volume.state = malloc( snake.volumesizeof ())
	return snake;
}

void snakeDestroy ( Snake* snake )
{
	free(snake->tmpSteps);
	free(snake->units);
	listSolutionDestroy(snake->solutions);
	free(snake);
}

void snakeAddSolution ( Snake* snake ){
	listSolutionInsert(snake->solutions,snake->tmpSteps);
	snake->tmpSteps = malloc ( snake->length * sizeof(Step) ); //Need to allocate space again to write next solution
}

Unit snakeGetNextUnit ( Snake* snake )
{
	snake->currentUnit ++;
	//Todo , error handling (index overflow)
	return snake->units[snake->currentUnit];

}
int snakeRewind ( Snake* snake){
	snake->currentUnit --;
	//Todo , error handling (index overflow)
	return snake->currentUnit;
}
void snakeAddStep ( Snake* snake, Step* step){
	//Todo, error handling (index overflow)
	memcpy( &(snake->tmpSteps[snake->currentUnit]), step,snake->length);
}
