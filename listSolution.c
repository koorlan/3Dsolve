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
    Solution *curr, *tmp;

    curr = listSolution->head;
    while(listSolution->size != 0)
    {
        tmp = curr;
        curr = curr->next;
        free(tmp);
        listSolution->size = listSolution->size - 1;
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
