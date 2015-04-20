#ifndef SNAKE_H
#define SNAKE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "listSolution.h"

/*
	Type énuméré des direction
	Utilisé pour caractériser l'orientation
	des éléments du snake
*/
typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK
} Dir;

/*
	Type énuméré Unit
	Caractérise le type d'un élément du snake
*/
typedef enum
{
	EDGE, // Extrémité
	STRAIGHT, // Unité droite
	CORNER // Unité d'angle
} Unit;

/*
	Coordonnée dans un espace 3D
*/
typedef struct coord
{
	int x;
	int y;
	int z;
} Coord;

/*
	Définit une étape de résolution
	Les étapes sont stockées dans un tableau, qui représente
	alors une solution
*/
typedef struct step
{
	Coord coord; // Coordonnée (dans le volume) de l'élément à placer
	Dir dir; // Orientation de l'élément à placer
} Step;

/*
	Type énuméré définisant l'état d'un élement du volume.
*/
typedef enum
{
	FORBIDDEN, // état interdi (permet des forme plus complexes que le cube)
	FREE, // état libre (on peut y positionner un élement du snake)
	BUSY // état occupé (un élément du snake y est déjà présent)
} VolumeState;

/*
	Volume "final"
	Définit la forme que la snake possède dans son état résolu
*/
typedef struct volume
{
	Coord max; // Dimention (x*y*z) du volume
	VolumeState ***state; // Tableau 3D définissant le volume

} Volume;


/*
	Snake, définit le snake à résoudre
*/
typedef struct snake
{
	int length // Nombre d'élement dans units
	Step* tmpSteps; // Solution en cour de recherche
	Unit* units; // Eléments unitaire du snake
	int currentUnit; // Index de l'élément en cour de traitement
	ListSolution* solutions; // Liste des solutions
	Volume volume; // Volume que le snake doit remplir pour être solutionné
} Snake;

/*
	Initialise un snake en lisant les donnée depuis
	le fichier spécifié par <templatePath>

	Valeurs de retour :
		- Le snake créé
		- null en cas d'échec
*/
Snake* snakeInit(char* templatePath);

/*
	Supprime proprement <snake> de la
	mémoire
*/
void snakeDestroy ( Snake* snake );

/*
	Copie la solution trouvé dans la liste
	des solutions de <snake>
*/
void snakeAddSolution ( Snake* snake );

/*
	Renvoie le type de la prochaine unité de <snake>
	qu'il faut placer dans le volume.
	Cette fontion assure également l'intégrité des données en
	gérant la value de l'entier "currentUnit"
*/
Unit snakeGetNextUnit ( Snake* snake );

/*
	Cette fonction permet de revenir en arrière dans
	<snake>. Tout comme "snakeGetNextUnit", elle gère
	l'intégrité des données.
*/
int snakeRewind ( Snake* snake);

/*
	Cette fonction ajoute une étape à la solution
	en cour de recherche.
	<step> sera écrit dans le tableau "tmpStep"
	à l'index "currentUnit" de <snake>
*/
void snakeAddStep ( Snake* snake, Step* step);

#endif //SNAKE_H
