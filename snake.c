#include "snake.h"

Snake* snakeInit(char* templatePath)
{
	FILE* file;
    file = fopen(templatePath, "r");
    if(file == NULL)
    {
		// Log error
		printf("Error on opening template file");
		return NULL;
    }

	Snake* snake = malloc ( sizeof(Snake) );

	// Loading volume size informations
	if(fscanf(file, "[Volume]\n%d;%d;%d\n", &(snake->volume.max).x,
		&(snake->volume.max).y, &(snake->volume.max).z) != 3)
	{
		// Log error
		printf("Error on reading volume size info");
		return NULL;
	}

	int i, j;
	const int stateNb = snake->volume.max.x*snake->volume.max.y*
								snake->volume.max.z;
	snake->volume.state = malloc(snake->volume.max.x * sizeof(VolumeState*));
	for(i = 0; i < snake->volume.max.x; i++)
		snake->volume.state[i] = malloc(snake->volume.max.y * sizeof(VolumeState*));
	for(i = 0; i < snake->volume.max.x; i++)
		for(j = 0; j < snake->volume.max.y; j++)
			snake->volume.state[i][j] = malloc(snake->volume.max.z * sizeof(VolumeState));

	int x = 0, y = 0, z = 0;
	int tmp;
	for(i=0; i < stateNb; i++)
	{
		x = i % snake->volume.max.x;
		y = (i / snake->volume.max.x) % snake->volume.max.y;
		z = (i / (snake->volume.max.x * snake->volume.max.y));
		if(fscanf(file, "%d;", &tmp) != 1)
		{
			printf("Error on reading volume state");
			return NULL;
		}

		snake->volume.state[x][y][z] = tmp;
	}

	if(fscanf(file, "\n[Snake]\n%d\n", &(snake->length)) != 1)
	{
		printf("Error on reading snake size");
		return NULL;
	}

	if(snake->length > 0)
		snake->units = malloc(snake->length * sizeof(Unit));

	for(i=0; i < snake->length; i++)
	{
		if(fscanf(file, "%d;", &tmp) != 1)
		{
			printf("Error on reading snake units");
			return NULL;
		}

		snake->units[i] = tmp;
	}

	fclose(file);

	snake->currentUnit = 0;
	snake->solutions = listSolutionCreate();
	snake->tmpSteps = malloc(snake->length * sizeof(Step));
	memset(snake->tmpSteps, 0,snake->length * sizeof(Step));

	/* Old initialization

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

*/
	printf("Snake loaded");

	int k;
	for(i = 0; i < snake->volume.max.x; i++)
		for(j = 0; j < snake->volume.max.y; j++)
			for(k = 0; k < snake->volume.max.z; k++)
				printf("%d", snake->volume.state[i][j][k]);

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
