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

/**
 * @file snake.h
 * @brief Structures et fonctions utilisées pour définir un snake
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
 * @defgroup Snake Snake
 *
 * @brief Module de gestion du Snake.
 *
 * Le module Snake s'articule principalement autour de la structure Snake qui
 * modélise le casse-tête.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "listSolution.h"
#include "log.h"

/**
 * @ingroup Snake
 * @enum Dir
 * @brief Direction, utilisé pour caractériser l'orientation
 * des éléments du snake
 */
typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	DNONE
} Dir;

/**
 * @ingroup Snake
 * @enum Unit
 * @brief Caractérise le type d'un élément du snake
 * @var Unit::EDGE
 * Elément de type extrémité
 * @var Unit::STRAIGHT
 * Elément de type droit, implique une conservation de la direction
 * @var Unit::CORNER
 * Elément de type angle, impique un changement de direction
 */
typedef enum
{
	EDGE,
	STRAIGHT,
	CORNER
} Unit;

/**
 * @ingroup Snake
 * @struct Coord
 * @brief Coordonnée dans un espace 3D
 * @var Coord::x
 * Coordonnée x
 * @var Coord::y
 * Coordonnée y
 * @var Coord::z
 * Coordonnée z
 */
typedef struct Coord
{
	int x;
	int y;
	int z;
} Coord;

/**
 * @ingroup Snake
 * @struct Step
 * @brief Définit une étape de résolution.
 *
 * Les étapes sont stockées dans un tableau, qui représente
 * alors une solution
 * @var Step::coord
 * Coordonnée (dans le volume) de l'élément à placer
 * @var Step::dir
 * Orientation de l'élément à placer
 */
struct Step
{
	Coord coord;
	Dir dir;
};

/**
 * @ingroup Snake
 * @enum VolumeState
 * @brief Type énuméré définisant l'état d'un élement du volume.
 * @var VolumeState::FORBIDDEN
 * état interdi (permet des forme plus complexes que le cube)
 * @var VolumeState::FREE
 * état libre (on peut y positionner un élement du snake)
 * @var VolumeState::FILL
 * état occupé (un élément du snake y est déjà présent)
 */
typedef enum
{
	FORBIDDEN,
	FREE,
	FILL
} VolumeState;

/**
 * @ingroup Snake
 * @struct Volume
 * @brief Définit la forme que le snake possède dans son état résolu
 * @var Volume::max
 * Dimention (x*y*z) du volume
 * @var state
 * Tableau 3D définisant le volume
 */
typedef struct Volume
{
	Coord max;
	VolumeState ***state;

} Volume;


/**
 * @ingroup Snake
 * @struct Snake
 * @brief Définit le snake à résoudre.
 *
 * Un snake est une suite d'éléments accroché les uns au autres.
 * Chaque élément possède un type précis affectant les possibilités
 * de rotation des éléments les uns par rapport aux autres. Résoudre le snake
 * consiste à trouver le bon enchainement des rotations ammenant les éléments
 * à remplir le volument définit par Snake::volume. Les éléments constituant le
 * snake sont définit par Snake::units.
 * @var Snake::length
 * Nombre d'élement dans units
 * @var Snake::tmpSteps
 * Solution en cour de recherche
 * @var Snake::units
 * Eléments unitaires du snake
 * @var Snake::currentUnit
 * Index de l'élément en cour de traitement
 * @var Snake::solutions
 * Liste des solutions
 * @var Snake::volume
 * Volume que le snake doit remplir pour être solutionné
 */
typedef struct Snake
{
	int length;
	Step* tmpSteps;
	Unit* units;
	int currentUnit;
	ListSolution* solutions;
	Volume volume;
	int symetries[4]; // Indique les différents axes de symétrie relatifs au snake
} Snake;

/**
 * @ingroup Snake
 * @brief Initialise un snake en lisant les donnée depuis
 * le fichier spécifier par <templatePath>
 * @param  templatePath Chemin absolu ou relatif à l'exécutable du fichier
 * à charger
 * @return              Le snake chargé, ou NULL si une erreur est survenue
 */
Snake* snakeInit(char* templatePath);

/**
 * @ingroup Snake
 * @breif Copie un snake.
 *
 * @param destination Destination de la Copie
 * @param source      Source de la copie
 */
void snakeCopy(Snake* destination, Snake* source);

/**
 * @ingroup Snake
 * @brief Supprime snake de la mémoire
 * @param snake Le snake à détruir
 * @param deleteSolution Si mit à 1, indique qu'il faut libérer de la mémoire
 * les solutions. Si mit à 0, les solution sont conservé en mémoire (utile
 * lors de la destruction des serpents utilisé par les threads)
 */
void snakeDestroy ( Snake* snake, int deleteSolution);

/**
 * @ingroup Snake
 * @brief Copie la solution trouvé (Snake::tmpSteps) dans la liste
 * des solution (Snake::solutions)
 * @param snake le snake concerné
 */
void snakeAddSolution ( Snake* snake );

/**
 * @ingroup Snake
 * @brief Renvoie le type de la prochaine unité de snake
 * qu'il faut placer dans le volume.
 *
 * Cette fontion assure également l'intégrité des données en
 * gérant la value de l'entier "currentUnit"
 * @param  snake le snake concerné
 * @return       la prochaine unité à traiter pour snake
 * @see snakeRewind
 */
Unit snakeGetNextUnit ( Snake* snake );

/**
 * @ingroup Snake
 * @brief Cette fonction permet de revenir en arrière dans
 * le snake. Elle est complémentaire à snakeGetNextUnit
 * @param  snake le snake concerné
 * @return       la nouvelle valeur de Snake::currentUnit
 * @see snakeGetNextUnit
 */
int snakeRewind ( Snake* snake);

/**
 * @ingroup Snake
 * @brief Ajoute une étape à la solution en cour de recherche.
 *
 * step sera écrit dans le tableau Snake::tmpSteps
 * à l'index Snake::currentUnit de snake
 * @param snake le snake concerné
 * @param step  l'étape à ajouter
 */
void snakeAddStep ( Snake* snake, Step* step);

/**
 * @ingroup Snake
 * @brief Imprimme le snake dans une chaine de caractère
 *
 * Renvoie une chaine de caractère de type
 * "ESCCSC...", étant une représentation "graphique"
 * du snake.

 * Il appartien à l'appellant de la fonction de libérer la chaine de caractère
 * de la mémoire.
 *
 * @param  snake le snake concerné
 * @return       La chaine de caractères représentant le snake
 */
char* snakePrint(Snake* snake);

/**
 * @ingroup Snake
 * @brief Affiche à l'écran les différentes solutions du snake
 * @param snake le snake concerné
 */
void snakePrintSolutions(Snake *snake);

#endif //SNAKE_H
