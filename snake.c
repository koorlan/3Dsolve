#include "snake.h"

Snake* snakeInit ()
{
	Snake* snake = malloc ( sizeof(Snake) );
	snake->length = 27;
	snake->tmpSteps = malloc ( 27 * sizeof(Step) );
	snake->units = malloc ( 27 * sizeof(Unit) );
	snake->currentUnit = 0;

	return snake;
}

void snakeDestroy ( Snake* snake )
{
	free(snake->tmpSteps);
	free(snake->units);
	free(snake);
}
