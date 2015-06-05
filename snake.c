/**
 * Projet application - 3DSolve
 * Version : 1.0
 *
 * Programme pour la résolution et la manipulation virtuelle de
 * casse-tête de type "Snake Cube"
 *
 * Auteurs :
 * 	- L.Aubry <lisa.aubry@insa-cvl.fr>
 * 	- A.Chazot <alban.chazot@insa-cvl.fr>
 * 	- K.Colas <korlan.colas@insa-cvl.fr>
 * 	- A.Gourd <anthony.gourd@insa-cvl.fr>
 *
 * Tuteur :
 * 	- P.Clemente <patrice.clemente@insa-cvl.fr>
 *
 * Date : 11 / 06 / 15
 *
 * INSA - Centre Val De Loire
 * 2014-2015
 * Promotion 2017
 *
 */

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
		snakeDestroy(snake, 1);
		return NULL;
	}

	// Loading volume
	int i, j;
	const int stateNb = snake->volume.max.x * snake->volume.max.y * snake->volume.max.z;
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
			snakeDestroy(snake, 1);
			return NULL;
		}
		snake->volume.state[x][y][z] = tmp;
	}


	// Loading snake
	if(fscanf(file, "\n[Snake]\n%d\n", &(snake->length)) != 1)
	{
		logError("[SNINI] Error on reading snake size\n");
		snakeDestroy(snake, 1);
		return NULL;
	}

	if(snake->length > 0)
		snake->units = malloc(snake->length * sizeof(Unit));

	for(i=0; i < snake->length; i++)
	{
		if(fscanf(file, "%d;", &tmp) != 1)
		{
			logError("[SNINI] Error on reading snake units\n");
			snakeDestroy(snake, 1);
			return NULL;
		}

		snake->units[i] = tmp;
	}

	// Loading symetry axis informations
	if(fscanf(file, "\n[Symetry]\n%d;%d;%d;%d",
	&(snake->symetries[0]), &(snake->symetries[1]),
	&(snake->symetries[2]), &(snake->symetries[3]) ) != 4)
	{
		logError("[SNINI] Error on reading symetry axis info\n");
		snakeDestroy(snake, 1);
		return NULL;
	}

	fclose(file);

	snake->currentUnit = 0;
	snake->solutions = listSolutionCreate();
	snake->tmpSteps = malloc(snake->length * sizeof(Step));
	memset(snake->tmpSteps, 0,snake->length * sizeof(Step));

	logWrite("[SNINI] Snake loaded (size : %d)\n", snake->length);

	return snake;
}

void snakeCopy(Snake* destination, Snake* source)
{
	destination->length = source->length;
	destination->currentUnit = 0;
	destination->solutions = listSolutionCreate();
	destination->tmpSteps = malloc(source->length * sizeof(Step));
	memset(destination->tmpSteps, 0,source->length * sizeof(Step));

	destination->volume.max.x = source->volume.max.x;
	destination->volume.max.y = source->volume.max.y;
	destination->volume.max.z = source->volume.max.z;

	int i, j ,k;

	destination->volume.state = malloc(destination->volume.max.x * sizeof(VolumeState*));
	for(i = 0; i < destination->volume.max.x; i++)
		destination->volume.state[i] = malloc(destination->volume.max.y * sizeof(VolumeState*));
	for(i = 0; i < destination->volume.max.x; i++)
		for(j = 0; j < destination->volume.max.y; j++)
		{
			destination->volume.state[i][j] = malloc(destination->volume.max.z * sizeof(VolumeState));
			for(k = 0; k < destination->volume.max.z; k++)
				destination->volume.state[i][j][k] = source->volume.state[i][j][k];
		}

	destination->units = malloc(source->length * sizeof(Unit));
	for(i = 0; i < source->length; i++)
		destination->units[i] = source->units[i];
}

void snakeDestroy ( Snake* snake, int deleteSolution)
{
	if(snake->tmpSteps != NULL)
		free(snake->tmpSteps);
	if(snake->units != NULL)
		free(snake->units);
	if(snake->solutions != NULL)
		listSolutionDestroy(snake->solutions, deleteSolution);

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
