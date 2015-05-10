#include "listSolution.h"

ListSolution* listSolutionCreate()
{
  ListSolution *listSolution = malloc ( sizeof (ListSolution) );
  listSolution->head = NULL;
  listSolution->size = 0;
  return listSolution;
}

void listSolutionDestroy(ListSolution* listSolution)
{
  Solution *curr,
           *tmp ;

  while ( listSolution->head != NULL)
  {
    curr = listSolution->head;
    tmp = curr;
    curr = curr->next;
    free(tmp);
    listSolution->head = curr;
  }

  free(listSolution);

}


void listSolutionInsert(ListSolution* listSolution, Step* steps)
{
    Solution* solution = malloc ( sizeof (Solution) );
    solution->step = steps;
    solution->next = listSolution->head;
    listSolution->head = solution;
    listSolution->size ++;
}
