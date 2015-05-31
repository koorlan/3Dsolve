/**
 * @defgroup Player
 *
 * @brief Ce module, articulé autour de la structure Player permet de modéliser
 * un joueur et de gérer l'interaction de celui-ci avec l'application.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "object.h"
#include "snake.h"
#include "linmath.h"

/**
 * @ingoup Player
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
 * @brief Calcule les coordonnées min et max du volume
 * @param  min    Pointeur pour récupérer les coord min
 * @param  max    Pointeur pour récupérer les coord max
 * @param  length ???
 * @param  player Le joueur
 * @param  volume ???
 * @return        ???
 */
int playerFindMinMax (Coord *min, Coord *max, int length, Player * player, Volume volume);

/**
 * @ingroup Player
 * @breif Vérifie si le joueur à résolu le casse tête ou non
 * @param  player Le joueur
 * @param  volume Le volume à remplir pour résoudre le casse-tête
 * @param  length La longueur du serpent
 * @return        1 si le joueur a résolu le casse-tête, -1 sinon.
 */
int playerCheckSolution (Player *player, Volume volume, int length);

/**
 * @ingroup Player
 * @brief ???
 * @param  solSnake ???
 * @param  snake    ???
 * @param  player   ???
 * @param  min      ???
 * @param  max      ???
 * @param  debugCpt ???
 * @return          ???
 */
int initVolume(Snake *solSnake, Snake *snake, Player * player, Coord min, Coord max, int *debugCpt);

/**
 * @ingroup Player
 * @brief Aide le joueur à résoudre le casse-tête à partir des mouvement qu'il a
 * commencé à faire
 * @param  player Le joueur
 * @param  snake  Le serpent à résoudre
 * @return        ???
 */
int playerHelp(Player *player, Snake * snake);

#endif //PLAYER_H
