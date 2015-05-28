#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "object.h"
#include "snake.h"
#include "linmath.h"



typedef struct player
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

Player* playerInit ( Snake* snake );
void playerFlatten ( Player* player, Snake* snake, int fromIndex );
void playerRotate ( Player* player, int stepIndex, Snake* snake, int magnet );
void playerFakeRotate ( Player* player, int stepIndex, Snake* snake, int magnet );
void playerDestroy ( Player* player );

//calcule les cordonnées min et max du volume
int playerFindMinMax (Coord *min, Coord *max, int length, Player * player, Volume volume);

//retourne 1 si le joueur a trouvé une solution, -1 sinon
int playerCheckSolution (Player *player, Volume volume, int length);

int initVolume(Snake *solSnake, Snake *snake, Player * player, Coord min, Coord max, int *debugCpt); 	

int playerHelp(Player *player, Snake * snake);

#endif //PLAYER_H
