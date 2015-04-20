#ifndef RESOLVER_H
#define RESOLVER_H

#include "snake.h"

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
Coord calcCoord(Coord coord,Dir dir);
int validCoord(Coord coord, Coord max);
#endif
