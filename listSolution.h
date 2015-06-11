/*
  Projet application - 3DSolve
  Version : 1.0

  Programme pour la résolution et la manipulation virtuelle de
  casse-tête de type "Snake Cube"

  Auteurs :
  	- L.Aubry <lisa.aubry@insa-cvl.fr>
  	- A.Chazot <alban.chazot@insa-cvl.fr>
  	- K.Colas <korlan.colas@insa-cvl.fr>
  	- A.Gourd <anthony.gourd@insa-cvl.fr>

  Tuteur :
  	- P.Clemente <patrice.clemente@insa-cvl.fr>

  Date : 11 / 06 / 15

  INSA - Centre Val De Loire
  2014-2015
  Promotion 2017

 */

/**
 * @file listSolution.h
 * @brief Structures et fonctions utilisées pour la manipulation et la stockage des solutions d'un snake
 * @authors Lisa Aubry <lisa.aubry@insa-cvl.fr>, Alban Chazot <alban.chazot@insa-cvl.fr>,
 * Korlan Colas <korlan.colas@insa-cvl.fr>, Anthony Gourd <anthony.gourd@insa-cvl.fr>
 * @date Juin 2015
 *
 * 3DSolve : un logiciel de résolution du casse-tête connu sous
 * le nom de Snake-Cube.
 * Projet tutoré par Patrice Clemente <patrice.clemente@insa-cvl.fr>
 *
 * INSA Centre Val de Loire : Année 2014-2015
 * Promotion 2017
 */

/**
 * @defgroup ListSolution ListSolution
 *
 * @brief Gère les liste de solution
 */

#ifndef LIST_SOLUTION_H
#define LIST_SOLUTION_H

#include <stdlib.h>

typedef struct Step Step;

/**
 * @ingroup ListSolution
 * @struct Solution
 * @brief Définit un élement d'une liste de solution
 * @var Solution::step
 * Un tabeau de Step qui décrit la solution
 * @var Solution::next
 * Pointeur vers la solution suivant, permet de créer le chainage et donc la liste
 */
typedef struct Solution
{
	Step* step;
	struct Solution *next;
} Solution;

/**
 * @ingroup ListSolution
 * @struct ListSolution
 * @brief Définit une liste de solution
 * @var ListSolution::head
 * Tête de liste, c'est un pointeur vers la première solution
 * @var ListSolution::size
 * Taille de la liste
 */
typedef struct ListSolution
{
	Solution* head;
	int size;
} ListSolution;

/**
 * @ingroup ListSolution
 * @brief Crée une liste de solution vide.
 * @return Un pointeur vers la liste créée
 */
ListSolution* listSolutionCreate();

/**
 * @ingroup ListSolution
 * @brief Détruit proprement un liste de solution
 * @param listSolution la liste à détruire.
 * @param deleteSteps si mis à 1, indique qu'il faut libérer de la mémoire
 * les tableau de solution. Sinon, seul la liste (le conteneur) est libéré
 */
void listSolutionDestroy(ListSolution* listSolution, int deleteSteps);

/**
 * @ingroup ListSolution
 * @brief Insère une solution dans une liste de solution.
 *
 * Cette fonction se charge de créer une nouvelle variable de type Solution
 * et affecte steps à Solution::step puis ajoute cette nouvelle solution à listSolution
 * @param listSolution la liste dans laquelle insérer la solution
 * @param steps        la solution à insérer
 */
void listSolutionInsert(ListSolution* listSolution, Step* steps);

#endif
