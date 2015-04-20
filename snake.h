#ifndef SNAKE_H
#define SNAKE_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "listSolution.h"

typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK
} Dir;

typedef enum
{
	EDGE,
	STRAIGHT,
	CORNER
} Unit;

typedef struct coord
{
	int x;
	int y;
	int z;
} Coord;

typedef struct step
{
	Coord coord;
	Dir dir;
} Step;

typedef enum
{
	FORBIDDEN,
	FREE,
	BUSY
} VolumeState;

typedef struct volume
{
	Coord max;
	VolumeState ***state;

} Volume;

typedef struct snake
{
	int length;
	Step* tmpSteps;
	Unit* units;
	int currentUnit;
	ListSolution* solutions;
	Volume volume;
} Snake;

Snake* snakeInit ();
void snakeDestroy ( Snake* snake );
void snakeAddSolution ( Snake* snake );
Unit snakeGetNextUnit ( Snake* snake );
int snakeRewind ( Snake* snake);
void snakeAddStep ( Snake* snake, Step* step);

#endif //SNAKE_H
