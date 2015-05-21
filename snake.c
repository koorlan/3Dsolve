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

	logWrite("[SNINI] Snake loaded (size : %d)\n", snake->length);

	return snake;
}

Snake* snakeCopy(Unit* units, Volume volume)
{
	Snake* snake = malloc ( sizeof(Snake) );
	snake->currentUnit = 0;
	snake->length = volume.max.x * volume.max.y * volume.max.z;
	snake->tmpSteps = malloc(snake->length * sizeof(Step));
	memset(snake->tmpSteps, 0,snake->length * sizeof(Step));
	snake->units = snakeCopyUnits(units, snake->length);
	snake->solutions = listSolutionCreate();
	snake->volume = snakeCopyVolume(volume);

	return snake;
}


Unit* snakeCopyUnits(Unit* units, int unitsNb)
{
	Unit* copiedUnits = malloc(unitsNb);
	int i;
	for(i = 0; i < unitsNb; i++)
		copiedUnits[i] = units[i];
	return copiedUnits;
}

Volume snakeCopyVolume(Volume volume)
{
	//logWrite("[TEST] max.x = %d; max.y = %d; max.z = %d", volume.max.x, volume.max.y, volume.max.z);
	Volume copiedVolume;
	copiedVolume.max.x = volume.max.x;
	copiedVolume.max.y = volume.max.y;
	copiedVolume.max.z = volume.max.z;

	int i, j, k;

	copiedVolume.state = malloc(copiedVolume.max.x * sizeof(VolumeState*));
	for(i = 0; i < copiedVolume.max.x; i++)
	{
		copiedVolume.state[i] = malloc(copiedVolume.max.y * sizeof(VolumeState*));
		for(j = 0; j < copiedVolume.max.y; j++)
		{
			copiedVolume.state[i][j] = malloc(copiedVolume.max.z * sizeof(VolumeState));
			for(k = 0; k < copiedVolume.max.z; k++)
				copiedVolume.state[i][j][k] = volume.state[i][j][k];
		}
	}

	return copiedVolume;
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

void snakeAddSolution ( Snake* snake )
{
	Step * tmp = malloc ( snake->length * sizeof(Step) );
	memset(tmp, 0,snake->length * sizeof(Step));
	memcpy(tmp , snake->tmpSteps, snake->length*sizeof(Step));
	listSolutionInsert(snake->solutions,tmp);
}

Unit snakeGetNextUnit ( Snake* snake )
{
	(snake->currentUnit)++;
	if(snake->currentUnit >= snake->length)
		snake->currentUnit = snake->length - 1;

	return snake->units[snake->currentUnit];
}

int snakeRewind ( Snake* snake){
	snake->currentUnit --;
	if(snake->currentUnit < 0)
		snake->currentUnit = 0;

	return snake->currentUnit;
}

void snakeAddStep ( Snake* snake, Step* step)
{
	if(snake->currentUnit < 0 || snake->currentUnit >= snake->length)
		return;
	memcpy( &(snake->tmpSteps[snake->currentUnit]), step, sizeof(Step));
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

void snakePrintSolutions(Snake *snake)
{
  int i;

  printf("\033[38;01mSnake resolved with\033[00m\033[31;01m %d \033[00m\033[38;01msolution(s) \033[00m\n", snake->solutions->size);

	Solution *curr = NULL;
	for (curr = snake->solutions->head; curr!= NULL ; curr = curr->next) {
	 	for ( i = 0; i < snake->length; i++) {
			printf("\n \033[33;01m %d \033[00m", i);
			switch (curr->step[i].dir)
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
	 	}
		printf("\n");
	}
}
