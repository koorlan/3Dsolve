#ifndef SNAKE_H
#define SNAKE_H

#include "stdlib.h"

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

typedef struct snake
{
	int length;
	Step* tmpSteps;
	Unit* units;
	int currentUnit;
	//TODO: Volume
} Snake;

Snake* snakeInit ();
void snakeDestroy ( Snake* snake );


#endif //SNAKE_H
