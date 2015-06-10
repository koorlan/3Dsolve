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
 * @defgroup Player Player
 *
 * @brief Ce module, articulé autour de la structure Player permet de modéliser
 * un joueur et de gérer l'interaction de celui-ci avec l'application.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "snake.h"
#include "linmath.h"

/**
 * @ingroup Player
 * @struct Player
 * @brief Modélise un joueur, soit un utilisateur de l'application.
 *
 * @var Player::steps
 * Tableau de résolution du joueur. Contient les différents mouvement que le joueur
 * a fait dans l'objectif de résoudre le casse-tête
 * @var Player::selected
 * Identifiant de l'élément du snake-cube sélectionné par le joueur
 * @var Player::currentSolution
 * ???
 * @var Player::realCubePos
 * ???
 * @var Player::realCubeRot
 * ???
 * @var Player::flatCubePos
 * ???
 * @var Player::segStart
 * Indentifiant du premier cube du segment sélectionné
 * @var Player::segEnd
 * Indentifiant du dernier cube du segment sélectionné
 */
typedef struct Player
{
	Step* steps;
	int selected;
	Solution* currentSolution;
	mat4x4* realCubePos;
	mat4x4* realCubeRot;
	vec3* flatCubePos;
	int segStart;
	int segEnd;
} Player;

#include "resolver.h"

Player* gplayer;
Player* gsolver;

/**
 * @ingroup Player
 * @brief Crée et initialise un Player.
 * @param  snake Le snake à résoudre
 * @return       Le Player créé.
 */
Player* playerInit ( Snake* snake );

/**
 * @ingroup Player
 * @brief ???
 * @param player    Le joueur
 * @param snake     Le serpent
 * @param fromIndex l'index de début
 */
void playerFlatten ( Player* player, Snake* snake, int fromIndex );

/**
 * @ingroup Player
 * @brief ???
 * @param player    Le joueur
 * @param stepIndex ???
 * @param snake     Le serpent
 * @param magnet    ???
 */
void playerRotate ( Player* player, int stepIndex, Snake* snake, int magnet );

/**
 * @ingroup Player
 * @brief ???
 * @param player    Le joueur
 * @param stepIndex [description]
 * @param snake     Le serpent
 * @param magnet    [description]
 */
void playerFakeRotate ( Player* player, int stepIndex, Snake* snake, int magnet );

/**
 * @ingroup Player
 * @brief Détruit un joueur et nettoie la mémoire
 * @param player Le joueur à détruir
 */
void playerDestroy ( Player* player );

/**
 * @ingroup Player
 * @brief Calcule les coordonnées min et max des unités placées par le joueur
 * @param  min    Pointeur pour récupérer les coord min
 * @param  max    Pointeur pour récupérer les coord max
 * @param  length La taille du serpent correspondant aux nombre d'unités déjà placées pas le joueur
 * @param  player Le joueur
 * @param  volume Le volume à remplir
  * @return       1 si ce que les unités déjà placées permettent potentiellement de résoudre le Snake, -1 sinon (les cubes se chevauchent ou bien sortent du volume)
 */
int playerFindMinMax (Coord *min, Coord *max, int length, Player * player, Volume volume);

/**
 * @ingroup Player
 * @brief Vérifie si le joueur à résolu le casse tête ou non
 * @param  player Le joueur
 * @param  volume Le volume à remplir pour résoudre le casse-tête
 * @param  length La longueur du serpent
 * @return        1 si le joueur a résolu le casse-tête, -1 sinon.
 */
int playerCheckSolution (Player *player, Volume volume, int length);

/**
 * @ingroup Player
 * @brief Remplit le volume qui servira à l'aie du joueur (volume à remplir moins les unités déjà placées par le joueur)
 * @param  solSnake Le snake pernettant de calculer les solutions
 * @param  snake    Le snake initial
 * @param  player   Le joueur
 * @param  min      Les valeurs minimum, sur les 3 axes, des unités placées
 * @param  max      Les valeurs maximum, sur les 3 axes, des unités placées
 * @param  debugCpt Un compteur d'entiers des valeur FILL dans le volume à initialiser (pour le debug)
 * @return          1 si le volume est bien initialisé, -1 si on se rend compte qu'aucune solution ne peut être trouvée
 */
int initVolume(Snake *solSnake, Snake *snake, Player * player, Coord min, Coord max, int *debugCpt);

/**
 * @ingroup Player
 * @brief Aide le joueur à résoudre le casse-tête à partir des mouvements qu'il a
 * déjà effectués
 * @param  player Le joueur
 * @param  snake  Le serpent à résoudre
 * @return        1 si des solutions sont trouvées, -1 sinon.
 */
int playerHelp(Player *player, Snake * snake);

#endif //PLAYER_H
