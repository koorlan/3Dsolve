/*
    3DSolve : un logiciel de résolution du casse-tête connue sous
    le nom de Snake-Cube.

    Projet tutoré par Patrice Clemente

    INSA Centre Val de Loire : Année 2014-2015
    Promotion 2017

    3DSolve Software Developement Team
    Lisa Aubry <lisa.aubry@insa-cvl.fr>
    Korlan Colas <korlan.colas@insa-cvl.fr>
    Alban Chazot <alban.chazot@insa-cvl.fr>
    Anthony Gourd <anthony.gourd@insa-cvl.fr>
*/

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
