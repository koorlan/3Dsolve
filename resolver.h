/**
 * @file resolver.h
 * @brief Structures et fonctions utilisées pour la réslution des "snake cube"
 * @authors Lisa Aubry <lisa.aubry@insa-cvl.fr>, Alban Chazot <alban.chazot@insa-cvl.fr>,
 * Korlan Colas <korlan.colas@insa-cvl.fr>, Anthony Gourd <anthony.gourd@insa-cvl.fr>
 * @date Juin 2015
 *
 * 3DSolve : un logiciel de résolution du casse-tête connu sous
 * le nom de Snake-Cube.
 * Projet tutoré par Patrice Clemente <patrice.clement@insa-cvl.fr>
 *
 * INSA Centre Val de Loire : Année 2014-2015
 * Promotion 2017
 */

#ifndef RESOLVER_H
#define RESOLVER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "log.h"
#include "snake.h"
#include "context.h"

#include "time.h"

/**
 * @struct NodeTree resolver.h
 * @brief Représente un noeud de l'arbre de résolution.
 * @var NodeTree::step
 * L'étape de la résolution portée par le noeud
 * @var NodeTree::parent
 * Pointeur sur le noeud parent
 * @var NodeTree::currentChild
 * Pointeur sur le fils courrant (le prochain noeud à procéder)
 * @var NodeTree::brother
 * Lien vers un frère (noeud au même niveau dans l'arbre, ayant le même père)
 * @var NodeTree::hasPlayed
 * Indique si les calculs pour le noeud ont été réalisé ou non
*/
typedef struct NodeTree
{
  Step step;
  struct NodeTree *parent;
  struct NodeTree *currentChild;
  struct NodeTree *brother;
  int hasPlayed;
} NodeTree;

/**
 * @struc FloatCoord
 * @brief Coordonnée en précision flotante.
 * @var FloatCoord::x
 * La coordonnée sur l'axe x
 * @var FloatCoord::y
 * La coordonnée sur l'axe y
 * @var FloatCoord::z
 * La coordonnée sur l'axe z
*/
typedef struct FloatCoord
{
  float x;
  float y;
  float z;
}FloatCoord;

/**
 * @struct Line
 * @brief Définit une ligne (ou axe de symétrie).
 *
 * Le type Line correspond à une droite définie par deux points
 * et la valeur des coefficients relatifs à son équation catésienne
 * (ax + by = c)
 * @var Line::a
 * Le coefficient "a" de la droite
 * @var Line::b
 * Le coefficient "b" de la droite
 * @var Line::c
 * Le coefficient "c" de la droite
 * @var Line::pointA
 * Le premier point utilisé pour définir la droite
 * @var Line::pointB
 * Le second point utilisé pour définir la droite
 */
typedef struct Line
{
  float a;
  float b;
  float c;
  FloatCoord pointA;
  FloatCoord pointB;
}Line;

/**
 * @typedef Tree
 * Tree est type définit comme étant un pointer sur NodeTree
 * @see NodeTree
 */
typedef NodeTree * Tree;

typedef struct ThreadArgs
{
    Tree rootNode;
    Snake* snake;
    pthread_mutex_t mutex;
} ThreadArgs;

/**
 * @brief Fonction principale dans la résolution du snake.
 * @param snake : le serpent à résoudre
 */
void resolverSolveSnake(Snake *snake);

void* resolverSolveNode(void* args);

/**
 * @brief Crée une nouvelle racine d'arbre.
 * @return Un pointeur sur la racine crée.
 */
Tree initTree();

/**
 * @brief Affiche un snake.
 *
 * Affiche dans la console les champs "units" qui composent le snake.
 * @param snake : le serpent à afficher
 */
void printSnake(Snake snake);

/**
 * @brief Effectue la copie d'une variable de type structure Step.
 * @param dest : la destination de la copie
 * @param src : la source de la copie
 */
void copyStep (Step * dest, Step src);

/**
 * @brief Ajoute un noeud dans un arbre.
 *
 * Créer un nouveau noeud et renseigne son champ Step grâce au paramètres envoyés.
 * Le nouveau noeud est accroché <rootNode>
 * @param rootNode : le noeud racine auquel sera attaché le nouveau noeud
 * @param x : la coordonnée x
 * @param y : la coordonnée y
 * @param z : la coordonnée z
 * @param newDir : la direction
 */
void addInitialVector(Tree rootNode, int x, int y, int z, Dir newDir);

/**
 * @brief Affiche les caractéristique de <currentNode> dans la console.
 *
 * Les caractéristique affichée sont les coordonnées ainsi
 * que la direction du cube qui a été posé à l'étape de la résolution
 * modélisé par <currentNode>
 * @param currentNode Le noeud à afficher
 */
void printCurrentNode(Tree currentNode);

/**
 * @brief Construit les enfants directs de <currentNode>.
 *
 * Lors du procésus de création des fils, les caractéristiques
 * de <snake> seront modifiée (elément courrant, et état du volume)
 * @param  currentNode Le noeud pour lequel on veux créer les fils
 * @param  snake       Le snake à résoudre.
 * @return             -1 si aucun fils ne peut être créé, 1 si au moins un fils a pu être créé et 2 si le dernier élement du snake a été placé.
 */
int buildChildren(Tree currentNode, Snake * snake);

/**
 * @brief Affiche <rootNode> dans la console.
 *
 * Fonctione de la même manière que printCurrentNode en affichant tous les
 * noeuds pour une hauteur donnée.
 * @param rootNode La racine de l'arbre à afficher
 * @see printCurrentNode
 */
void printTree (Tree rootNode);

/**
 * @brief Affiche les caractéristique de <line> dans la console.
 *
 * Les caractéristiques affichées sont :
 * 	- les coordonnées des deux point utilisés pour définir la droite
 * 	- les valeurs des coefficients dans son écriture cartésienne
 * @param line Ligne que l'on veut afficher
 */
void printLine (Line line);

/**
 * @brief Calcul l'équation linéaire de <line>.
 * @param line Ligne dont on veut calculer l'équation linéaire
 */
void linearEquation (Line *line);

/**
 * @brief Vérifie si <srcDir> et <destDir> sont opposées par rapport à <typeOfAxis>.
 * @param  srcDir     1er destination à comparer
 * @param  destDir    2eme destination à comparer
 * @param  typeOfAxis type de l'axe de sysmétrie utilisé pour la compéraison
 * @return            1 si les directions sont opposée, 0 sinon
 */
int oppositeDir(Dir srcDir, Dir destDir, char * typeOfAxis);

/**
 * @brief Copie <src> dans <dest>.
 * @param dest Destination de la copie
 * @param src  Source de la copie
 */
void cpyLine(Line *dest, Line src);

/**
 * @brief Détermine si un vecteur est symétrique à un autre selon les différentes symétries axiales dans un plan.
 * @param  initialStep    [description]
 * @param  nCoord         [description]
 * @param  nDir           [description]
 * @param  verticalAxis   [description]
 * @param  horizontalAxis [description]
 * @param  diagonalAxis   [description]
 * @param  slashAxis      [description]
 * @return                [description]
 */
int symmetries (Step initialStep, Coord nCoord, Dir nDir, Line verticalAxis, Line horizontalAxis, Line diagonalAxis, Line slashAxis);

/**
 * @brief Construit une liste de vecteurs initiaux dans un volume donné.
 *
 * - On détermine le centre du volume
 * - On considère tour à tour chacune des faces de cette figure
 * - On projette orthogonalement le centre de la figure sur une face
 * - On construit les axes de symétrie passant par le centre
 * - On peut alors comparer les vecteurs deux à deux pour ne garder que ceux qui n'ont pas déjà un symétrique
 * @param  volume Le volume sur lequel effectuer le calcul
 * @param vectorNb un pointer sur int pour stocker le nombre de vecteur initiaux
 * @return        L'arbre contenant les vecteurs initiaux
 */
Tree findInitialVectors(Volume volume, int* vectorNb);

/**
 * @brief Calcul des coordonnées en fonction d'une direction.
 * @param  coord La coordonnée de départ
 * @param  dir   La direction dans laquelle se déplacer
 * @return       La coordonnée calculée
 */
Coord calcCoord(Coord coord,Dir dir);

/**
 * @brief Vérifie la validé de <coord>.
 *
 * Des coordonnées sont valides si pour chacune de leurs
 * composante, sa valeur est comprise entre 0 et la valeur maximale
 * donnée par <max>
 * @param  coord Les coordonnées à vérifier
 * @param  max   Les coordonnées maximales
 * @return       1 si <coord> est valide, 0 sinon
 */
int validCoord(Coord coord, Coord max);

/**
 * @brief Même prinicipe que la fonction valideCoord mais avec des inégalitées larges.
 * @param  coord Les coordonnées à vérifier
 * @param  max   Les coordonnées maximales
 * @return       1 si <coord> est valide, 0 sinon
 * @see valideCoord
 */
int validCoordSym(Coord coord, FloatCoord max);

/**
 * @brief Crée tous les points de départ possible pour un volume donné.
 *
 * Cette fonction permet de pouvoir chercher les solutions à partir de la
 * liste exhaustive des points de départ possibles. Elle permet nottament de
 * mettre en évidence l'efficacité de la méthode de réduction des points de
 * départ par comparaison des temps d'exécution des deux méthodes.
 * @param  volume Le volume étudié
 * @return        Pointeur vers la racine de l'arbre à laquelle sont attaché
 * les vecteurs initiaux
 */
Tree createAllInitialVectors(Volume volume);


#endif // RESOLVER_H
