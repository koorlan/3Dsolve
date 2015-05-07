#include "snake.h"

Snake* snakeInit(char* templatePath)
{
	FILE* file;
    file = fopen(templatePath, "r");
    if(file == NULL)
    {
		logError("[SNINI] Error on opening template : %s\n", templatePath);
		return NULL;
    }

	Snake* snake = malloc ( sizeof(Snake) );
	snake->tmpSteps = NULL;
	snake->units = NULL;
	snake->solutions = NULL;
	snake->volume.state = NULL;

	// Loading volume size informations
	if(fscanf(file, "[Volume]\n%d;%d;%d\n", &(snake->volume.max.x),
		&(snake->volume.max.y), &(snake->volume.max.z)) != 3)
	{
		logError("[SNINI] Error on reading volume size info\n");
		snakeDestroy(snake);
		return NULL;
	}

	// Loading volume
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
		if(fscanf(file, "%d;%d;%d;%d", &x, &y, &z, &tmp) != 4)
		{
			logError("[SNINI] Error on reading volume state\n");
			snakeDestroy(snake);
			return NULL;
		}
		snake->volume.state[x][y][z] = tmp;
	}


	// Loading snake
	if(fscanf(file, "\n[Snake]\n%d\n", &(snake->length)) != 1)
	{
		logError("[SNINI] Error on reading snake size\n");
		snakeDestroy(snake);
		return NULL;
	}

	if(snake->length > 0)
		snake->units = malloc(snake->length * sizeof(Unit));

	for(i=0; i < snake->length; i++)
	{
		if(fscanf(file, "%d;", &tmp) != 1)
		{
			logError("[SNINI] Error on reading snake units\n");
			snakeDestroy(snake);
			return NULL;
		}

		snake->units[i] = tmp;
	}

	fclose(file);

	snake->currentUnit = 0;
	snake->solutions = listSolutionCreate();
	snake->tmpSteps = malloc(snake->length * sizeof(Step));
	memset(snake->tmpSteps, 0,snake->length * sizeof(Step));

	logWrite("[SNINI] Snake loaded\n");

	return snake;
}

void snakeDestroy ( Snake* snake )
{
	if(snake->tmpSteps != NULL)
		free(snake->tmpSteps);
	if(snake->units != NULL)
		free(snake->units);
	if(snake->solutions != NULL)
		listSolutionDestroy(snake->solutions);

	if(snake->volume.state != NULL)
	{
		int i, j;
		for(i = 0; i < snake->volume.max.x; i++)
		{
			for(j = 0; j < snake->volume.max.y; j++)
				free(snake->volume.state[i][j]);
			free(snake->volume.state[i]);
		}
		free(snake->volume.state);
		logWrite("[SNDES] Volume freed\n");
	}

	free(snake);
}

void snakeAddSolution ( Snake* snake ){
	Step * tmp = malloc ( snake->length * sizeof(Step) );
	memset(tmp, 0,snake->length * sizeof(Step));
	memcpy(tmp , snake->tmpSteps,snake->length*sizeof(Step));
	listSolutionInsert(snake->solutions,snake->tmpSteps);
	snake->tmpSteps = tmp; //Need to allocate space again to write next solution

}

Unit snakeGetNextUnit ( Snake* snake )
{
	(snake->currentUnit)++;
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
	//Todo , error handling (index overflow)
	memcpy( &(snake->tmpSteps[snake->currentUnit]), step,sizeof(Step));  //maybe an error here
}

char* snakePrint(Snake* snake)
{
	char* snakeString = malloc(snake->length);
	int i;
	for(i = 0; i < snake->length; i++)
	{
		switch(snake->units[i])
		{
			case EDGE:
				snakeString[i] = 'E';
				break;
			case STRAIGHT:
				snakeString[i] = 'S';
				break;
			case CORNER:
				snakeString[i] = 'C';
				break;
			default:
				snakeString[i] = '-';
				break;
		}
	}

	return snakeString;
}

void snakePrintSolutions(int nbSolutions, ListSolution * snakeSolutions, int snakeLength)
{
  int i; 

  if (snakeSolutions->head!=NULL)
  {
  	ListSolution *tmpSol = malloc(sizeof(ListSolution));
    memset(tmpSol, 0, sizeof(snakeSolutions));
  	memcpy(tmpSol, snakeSolutions, sizeof(snakeSolutions));
    while(tmpSol->head != NULL)
    {
      i=0;
      while(i < snakeLength)
      {
        printf("\n \033[33;01m %d \033[00m", i);
        switch (tmpSol->head->step[i].dir)
        {
          case UP:
            printf("UP ");
            break;
          case DOWN:
            printf("DOWN ");
            break;
          case LEFT:
            printf("LEFT ");
            break;
          case RIGHT:
            printf("RIGHT ");
            break;
          case FRONT:
            printf("FRONT ");
            break;
          case BACK:
            printf("BACK ");
            break;
          case DNONE:
          default :
            printf("Error in solutions string\n");
            exit(-1);
        }
        i++;
      }
      printf("\n");
      tmpSol->head = tmpSol->head->next;
    }
  }
}
