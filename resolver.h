#ifndef RESOLVER_H
#define RESOLVER_H

#include "snake.h"

typedef struct nodeTree
{
  Step step;
  struct nodeTree *parent;
  struct nodeTree *currentChild;
  struct nodeTree *brother;
  int hasPlayed;
} NodeTree;

void resolverSolveSnake(Snake *snake);

#endif
