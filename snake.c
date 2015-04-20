#include "snake.h"

Snake* snakeInit ()
{

	Snake* snake = malloc ( sizeof(Snake) );
	snake->length = 27; //ne to be read from the template file
	snake->tmpSteps = malloc ( snake->length * sizeof(Step) );
	memset(snake->tmpSteps, 0,snake->length * sizeof(Step) );
	snake->units = malloc ( snake->length * sizeof(Unit) );
	memset(snake->units, 0,snake->length *  sizeof(Unit) );
	snake->currentUnit = 0;
	snake->solutions = listSolutionCreate();
	snake->volume.max.x = 3;
	snake->volume.max.y = 3;
	snake->volume.max.z = 3;

  snake->volume.state = malloc( snake->volume.max.x * sizeof (*snake->volume.state) );

	int i,j;
	for ( i = 0; i < snake->volume.max.x; i++)
	{
		snake->volume.state[i] = malloc( snake->volume.max.y * sizeof (**snake->volume.state) );
	}
	for ( i = 0; i < snake->volume.max.x; i++)
	{
		for ( j = 0; j < snake->volume.max.y; j++)
		{
			snake->volume.state[i][j] = malloc( snake->volume.max.z * sizeof (***snake->volume.state) );
		}
	}


	return snake;
}

void snakeDestroy ( Snake* snake )
{
	free(snake->tmpSteps);
	free(snake->units);
	listSolutionDestroy(snake->solutions);

	int i,j;
	for ( i = 0; i < snake->volume.max.x; i++)
	{
		for ( j = 0; j < snake->volume.max.y; j++)
		{
			free(snake->volume.state[i][j]);
		}
	}
	for ( i = 0; i < snake->volume.max.x; i++)
	{
		free(snake->volume.state[i]);
	}
	free(snake->volume.state);

	free(snake);
}

void snakeAddSolution ( Snake* snake ){
	Step * tmp = malloc ( snake->length * sizeof(Step) );
	memset(tmp, 0,snake->length * sizeof(Step));
	memcpy( tmp , snake->tmpSteps,snake->length*sizeof(Step));
	listSolutionInsert(snake->solutions,snake->tmpSteps);
	snake->tmpSteps = tmp; //Need to allocate space again to write next solution
}

Unit snakeGetNextUnit ( Snake* snake )
{
	snake->currentUnit ++;
	//Todo , error handling (index overflow)
	return snake->units[snake->currentUnit];

}
int snakeRewind ( Snake* snake){
	snake->currentUnit --;
	if(snake->currentUnit < 0)
		snake->currentUnit = 0;
	//Todo , error handling (index overflow)
	return snake->currentUnit;
}
void snakeAddStep ( Snake* snake, Step* step){
	//Todo, error handling (index overflow)
	memcpy( &(snake->tmpSteps[snake->currentUnit]), step,sizeof(Step));  //maybe an error here
}
