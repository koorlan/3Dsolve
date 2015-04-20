#ifndef SNAKE_H
#define SNAKE_H
#include <string.h>
#include <stdlib.h>
#include "listSolution.h"

typedef enum
{
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_FRONT,
	D_BACK,
	D_NONE
} Dir;

typedef enum
{
	EXT,
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
