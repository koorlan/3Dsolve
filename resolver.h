/*
    3DSolve : un logiciel de résolution du casse-tête connu sous
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

typedef struct 
{
  float x;
  float y;
  float z;
}FloatCoord;

typedef struct {
  float a;
  float b;
  float c;
  FloatCoord pointA;
  FloatCoord pointB;
}Line;

typedef NodeTree * Tree; 


/*
  Fonction principale dans la résolution du snake
*/
void resolverSolveSnake(Snake *snake);

/*
  Initialise initialise la racine d'un arbre n-aire 
  La racine est un élément "dummy",
  permettant simplement d'accéder à l'arbre
*/
void initTree(Tree * rootNode);

/*
  Affichage console des champs "units" qui composent le snake 
*/
void printSnake(Snake snake);

/*
  Copie un vecteur (Step) source dans un vecteur destination
  Seront copiés :
    - Un n-uplet de coordonnées
    - Sa direction 
*/
void copyStep (Step * dest, Step src);

/*
 Ajoute les différents vecteurs initiaux à la hauteur 1 de l'arbre 
*/
void addInitialVector(Tree * rootNode, int x, int y, int z, Dir newDir);

/*
  Affichage console du noeud de l'arbre courant
  Seront listés : 
    - Le n-uplet correspondant aux coordonnées du noeud
    - La direction 
*/
void printCurrentNode(Tree currentNode);

/*
  A partir d'un noeud donné et du snake, cette procédure : 
    - Calcule les fils du noeud courant
    - Accroche les fils au noeud courant
    - Ajoute le noeud courant dans la liste solution courante
    - Retire les coordonnées du noeud courant dans le volume à remplir
*/
int buildChildren(Tree * currentNode, Snake * snake);

/* 
  Idem que printCurrentNode étendu à tous les noeud sur une hauteur donnée 
*/
void printTree (Tree rootNode);


/*
  Affiche les composantes d'une droite :
    - 2 points 
    - La valeur des coefficients dans son écriture cartésienne
*/
void printLine (Line line);


/*
  Définit l'équation d'une droite à partir de deux points
*/
void linearEquation (Line *line); 

/*
  Retourne 1 si deux directions sont opposées selon une symétrie définie par typeOfAxis
*/
int oppositeDir(Dir srcDir, Dir destDir, char * typeOfAxis);


/*
  Recopie les champs de src dans dest. 
  Le type Line correspond à une droite définie par deux points et la valeur des coefficients relatifs à son équation catésienne
*/
void cpyLine(Line *dest, Line src);


/*
  Détermine si un vecteur est symétrique à un autre selon les différentes symétries axiales dans un plan
*/
int symmetries (Step initialStep, Coord nCoord, Dir nDir, Line verticalAxis, Line horizontalAxis, Line diagonalAxis, Line slashAxis);


/*
  Construit une liste de vecteurs initiaux dans un volume donné, pour cela : 
    - On détermine le centre du volume
    - On considère tour à tour chacune des faces de cette figure
    - On projette orthogonalement le centre de la figure sur une face
    - On construit les axes de symétrie passant par le centre
    - On peut alors comparer les vecteurs deux à deux pour ne garder que ceux qui n'ont pas déjà un symétrique  
*/
Tree findInitialVectors(Volume volume);


/*
  A partir d'une direction et d'un n-uplet de coordonnées,
  retourne des coordonnées correspondant au cube suivant dans le snake
*/
Coord calcCoord(Coord coord,Dir dir);


/*
  Retourne 1 si des coordonnées appartiennent au volume à remplir,
  0 sinon
*/
int validCoord(Coord coord, Coord max);


/*
  Idem que précédemment mais avec des inégalités larges
*/
int validCoordSym(Coord coord, FloatCoord max);


#endif
