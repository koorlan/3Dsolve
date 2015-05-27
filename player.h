#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "context.h"
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

Player* gplayer;
Player* gsolver;

Player* playerInit ( Snake* snake );
void playerFlatten ( Player* player, Snake* snake, int fromIndex );
void playerRotate ( Player* player, int stepIndex, Snake* snake, int magnet );
void playerFakeRotate ( Player* player, int stepIndex, Snake* snake, int magnet );
void playerDestroy ( Player* player );

#endif //PLAYER_H
