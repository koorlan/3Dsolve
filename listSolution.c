/**
 * Projet application - 3DSolve
 * Version : 1.0
 *
 * Programme pour la résolution et la manipulation virtuelle de
 * casse-tête de type "Snake Cube"
 *
 * Auteurs :
 * 	- L.Aubry <lisa.aubry@insa-cvl.fr>
 * 	- A.Chazot <alban.chazot@insa-cvl.fr>
 * 	- K.Colas <korlan.colas@insa-cvl.fr>
 * 	- A.Gourd <anthony.gourd@insa-cvl.fr>
 *
 * Tuteur :
 * 	- P.Clemente <patrice.clemente@insa-cvl.fr>
 *
 * Date : 11 / 06 / 15
 *
 * INSA - Centre Val De Loire
 * 2014-2015
 * Promotion 2017
 *
 */

#include "listSolution.h"

ListSolution* listSolutionCreate()
{
  ListSolution *listSolution = malloc ( sizeof (ListSolution) );
  listSolution->head = NULL;
  listSolution->size = 0;
  return listSolution;
}

void listSolutionDestroy(ListSolution* listSolution, int deleteSteps)
{
    Solution *curr, *tmp;

    curr = listSolution->head;
    while(listSolution->size != 0)
    {
        tmp = curr;
        curr = curr->next;
        if(deleteSteps)
            free(tmp->step);
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
