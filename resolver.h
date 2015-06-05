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
 * @file resolver.h
 * @brief Structures et fonctions utilisées pour la réslution des "snake cube"
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
 * @defgroup Resolver Resolver
 *
 * @brief Module gérant la résolution d'un snake cube.
 *
 * Ce module permet le calcul de la solution d'un snake. Il calcul les sysmétries
 * du volume afin d'éliminer un maximum de point de départ puis cherche les différentes
 * solutions possibles par un parcourt en profondeur du graphe modélisant tous les
 * chemins possibles.
 */

#ifndef RESOLVER_H
#define RESOLVER_H

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
	#include <windows.h>
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // Other kinds of Mac OS
    #endif
#elif __linux
    #include <unistd.h>
#else
		#error "Unspported Plateform"
#endif

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
 * @ingroup Resolver
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
 * @ingroup Resolver
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
 * @ingroup Resolver
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
 * @ingroup Resolver
 * @typedef Tree
 * Tree est type définit comme étant un pointer sur NodeTree
 * @see NodeTree
 */
typedef NodeTree * Tree;

/**
 * @ingroup Resolver
 * @struct ThreadArgs
 * @brief Structure utilisé pour passer les paramètres aux thread de calcul.
 *
 * @var ThreadArgs::rootNode
 * Le noeud à partir duquel le thread doit chercher les solutions
 * @var ThreadArgs::snake
 * Le snake à résoudre
 * @var ThreadArgs::exploredWayNb
 * Pointeur sur entier qui sera valuer par le thread afin d'informer le
 * thread principal du nombre de chemin explorés.
 */
typedef struct ThreadArgs
{
    Tree rootNode;
    Snake* snake;
	int exploredWayNb;
	int resetSolutionMenu;
} ThreadArgs;

/**
 * @ingroup Resolver
 * @brief Fonction principale dans la résolution du snake.
 * @param snake le serpent à résoudre
 */
void* resolverSolveSnake(void* arg);


/**
 * @ingroup Resolver
 * @brief Cette fonction cherche les solutions à partir du noeud qui lui est
 * passé en paramètre.
 *
 * Cette fonction est la fonction principale des thread de calcul. Elle tente
 * de résoudre le serpent à partir du noued qui lui est donné en paramètre. Les
 * informations sont remontées au thread principal via les pointeurs présents
 * dans la structure ThreadArgs
 * @param args  les arguments du thread
 * @see ThreadArgs
 */
void* resolverSolveNode(void* args);

/**
 * @ingroup Resolver
 * @brief Crée une nouvelle racine d'arbre.
 * @return Un pointeur sur la racine crée.
 */
Tree initTree();

/**
 * @ingroup Resolver
 * @brief Affiche un snake.
 *
 * Affiche dans la console les champs "units" qui composent le snake.
 * @param snake : le serpent à afficher
 */
void printSnake(Snake snake);

/**
 * @ingroup Resolver
 * @brief Effectue la copie d'une variable de type structure Step.
 * @param dest : la destination de la copie
 * @param src : la source de la copie
 */
void copyStep (Step * dest, Step src);

/**
 * @ingroup Resolver
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
 * @ingroup Resolver
 * @brief Affiche les caractéristique de <currentNode> dans la console.
 *
 * Les caractéristique affichée sont les coordonnées ainsi
 * que la direction du cube qui a été posé à l'étape de la résolution
 * modélisé par <currentNode>
 * @param currentNode Le noeud à afficher
 */
void printCurrentNode(Tree currentNode);

/**
 * @ingroup Resolver
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
 * @ingroup Resolver
 * @brief Affiche <rootNode> dans la console.
 *
 * Fonctione de la même manière que printCurrentNode en affichant tous les
 * noeuds pour une hauteur donnée.
 * @param rootNode La racine de l'arbre à afficher
 * @see printCurrentNode
 */
void printTree (Tree rootNode);



int cptNode (Tree rootNode);


/**
 * @ingroup Resolver
 * @brief Affiche les caractéristique de <line> dans la console.
 *
 * Les caractéristiques affichées sont :
 * 	- les coordonnées des deux point utilisés pour définir la droite
 * 	- les valeurs des coefficients dans son écriture cartésienne
 * @param line Ligne que l'on veut afficher
 */
void printLine (Line line);

/**
 * @ingroup Resolver
 * @brief Calcul l'équation linéaire de <line>.
 * @param line Ligne dont on veut calculer l'équation linéaire
 */
void linearEquation (Line *line);

/**
 * @ingroup Resolver
 * @brief Vérifie si <srcDir> et <destDir> sont opposées par rapport à <typeOfAxis>.
 * @param  srcDir     1er destination à comparer
 * @param  destDir    2eme destination à comparer
 * @param  typeOfAxis type de l'axe de sysmétrie utilisé pour la compéraison
 * @return            1 si les directions sont opposée, 0 sinon
 */
int oppositeDir(Dir srcDir, Dir destDir, char * typeOfAxis);

/**
 * @ingroup Resolver
 * @brief Copie <src> dans <dest>.
 * @param dest Destination de la copie
 * @param src  Source de la copie
 */
void cpyLine(Line *dest, Line src);

/**
 * @ingroup Resolver
 * @brief Détermine si un vecteur est symétrique à un autre selon les différentes symétries axiales dans un plan.
 *
 * @param  initialStep    Un vecteur de départ à partir on calcul les symétries
 * @param  nCoord         Coordonnée du vecteur étudié
 * @param  nDir           Direction du vecteur étudié
 * @param  verticalAxis   Axe vertical
 * @param  horizontalAxis Axe horizontal
 * @param  diagonalAxis   Axe diagonal
 * @param  slashAxis      Second axe diagonal
 * @return                1 si le vecteur est symétrique, 0 sinon
 */
int symmetries (Step initialStep, Coord nCoord, Dir nDir, Line verticalAxis, Line horizontalAxis, Line diagonalAxis, Line slashAxis);

/**
 * @ingroup Resolver
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
Tree findInitialVectors(Snake *snake, int* initialVectorNb);

/**
 * @ingroup Resolver
 * @brief Calcul des coordonnées en fonction d'une direction.
 * @param  coord La coordonnée de départ
 * @param  dir   La direction dans laquelle se déplacer
 * @return       La coordonnée calculée
 */
Coord calcCoord(Coord coord,Dir dir);

/**
 * @ingroup Resolver
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
 * @ingroup Resolver
 * @brief Même prinicipe que la fonction valideCoord mais avec des inégalitées larges.
 * @param  coord Les coordonnées à vérifier
 * @param  max   Les coordonnées maximales
 * @return       1 si <coord> est valide, 0 sinon
 * @see valideCoord
 */
int validCoordSym(Coord coord, FloatCoord max);

/**
 * @ingroup Resolver
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

/**
 * @ingroup Resolver
 * @brief Permet d'indenfier des vecteurs valides pour le recherche de sysmétrie.
 *
 * Dans le cas où le "tranche" étudié est une "tranche" intérieur, on ne veut pas
 * étudier les vecteur extérieur car ils sont déjà considéré dans l'étude des
 * faces extérieures.
 * @param  nCoord Les coordonnées du vecteur à valideCoord
 * @param  dir    La direction du vecteur à valider
 * @param  max    La taille du cube
 * @return        1 si le vecteur est valide, 0 sinon
 */
int validVectCube(Coord nCoord, Dir dir, int max);

int resolverInitializeHelp(Snake *snake, Step fstStep);


#endif // RESOLVER_H
