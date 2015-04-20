#ifndef RESOLVER_H
#define RESOLVER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "snake.h"
#include "context.h"

 const int cornerTruthTable[6][6] =
  {
    {0 , 0 , 1 , 1 , 1 , 1 },
    {0 , 0 , 1 , 1 , 1 , 1 },
    {1 , 1 , 0 , 0 , 1 , 1 },
    {1 , 1 , 0 , 0 , 1 , 1 },
    {1 , 1 , 1 , 1 , 0 , 0 },
    {1 , 1 , 1 , 1 , 0 , 0 }
  } ;

typedef struct nodeTree
{
  Step step;
  struct nodeTree *parent;
  struct nodeTree *currentChild;
  struct nodeTree *brother;
  int hasPlayed;
} NodeTree;

void resolverSolveSnake(Snake *snake);
void resolverFindSymmetry(Volume volume);
Coord calcCoord(Coord coord,Dir dir);
int validCoord(Coord coord, Coord max);
#endif
