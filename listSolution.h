#ifndef LIST_SOLUTION_H
#define LIST_SOLUTION_H

#include <stdlib.h>

typedef struct step Step;

typedef struct solution
{
	Step* step;
	struct solution *next;
} Solution;

typedef struct listSolution
{
	Solution* head;
	int size;
} ListSolution;

ListSolution* listSolutionCreate();

void listSolutionDestroy();

void listSolutionInsert();

#endif
