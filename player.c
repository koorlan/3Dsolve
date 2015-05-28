#include "player.h"

//UP,DOWN,LEFT,RIGHT,FRONT,BACK
//Axe en deuxième position
const Dir rotationTable[6][6] =
{
{ UP, UP, FRONT, BACK, RIGHT, LEFT },
{ DOWN, DOWN, BACK, FRONT, LEFT, RIGHT },
{ FRONT, BACK, LEFT, LEFT, UP, DOWN },
{ BACK, FRONT, RIGHT, RIGHT, DOWN, UP },
{ RIGHT, LEFT, DOWN, UP, FRONT, FRONT },
{ LEFT, RIGHT, UP, DOWN, BACK, BACK }
};

//x,y,z
const int dir2int[6][3] = 
{
{ 0, 1, 0 },
{ 0,-1, 0 },
{-1, 0, 0 },
{ 1, 0, 0 },
{ 0, 0, 1 },
{ 0, 0,-1 }
};

const float dir2vec[6][3] = 
{
{  0.f ,  1.f ,  0.f  },
{  0.f , -1.f ,  0.f  },
{ -1.f ,  0.f ,  0.f  },
{  1.f ,  0.f ,  0.f  },
{  0.f ,  0.f ,  1.f  },
{  0.f ,  0.f , -1.f  }
};


Player* playerInit ( Snake* snake )
{
	Player* player = malloc ( sizeof (Player) );
	player->steps = malloc ( snake->length * sizeof(Step) );
	player->selected = -1;
	player->steps[0].dir = RIGHT;
	player->currentSolution = NULL;

	playerFlatten ( player, snake, 0 );

	player->realCubePos = malloc ( snake->length * 16 * sizeof(float) );
	player->realCubeRot = malloc ( snake->length * 16 * sizeof(float) );
	player->flatCubePos = malloc ( snake->length * 3 * sizeof(float) );
	int i;
	for (i=0;i<snake->length;i++)
	{
		mat4x4_translate(player->realCubePos[i],
			(float) player->steps[i].coord.x,
			(float) player->steps[i].coord.y,
			(float) player->steps[i].coord.z);
		mat4x4_identity(player->realCubeRot[i]);
		player->flatCubePos[i][0] = (float) player->steps[i].coord.x;
		player->flatCubePos[i][1] = (float) player->steps[i].coord.y;
		player->flatCubePos[i][2] = (float) player->steps[i].coord.z;
	}

	player->segStart = -1;
	player->segEnd = -1;

	return player;
}

void playerFlatten ( Player* player, Snake* snake, int fromIndex )
{

	Dir dir = player->steps[fromIndex].dir;
	Dir cstDir = dir;
	Dir othDir = DNONE;
	switch ( cstDir )
	{
		case UP		: othDir = RIGHT; break;
		case DOWN	: othDir = LEFT; break;
		case LEFT	: othDir = FRONT; break;
		case RIGHT	: othDir = BACK; break;
		case FRONT	: othDir = LEFT; break;
		case BACK	: othDir = RIGHT; break;
		default		: othDir = DNONE; break;
	}

	int i;
	//for ( i = 0; i < snake->length; i++ )
	for ( i = fromIndex; i < snake->length; i++ )
	{
		//direction
		if ( snake->units[i] == CORNER )
			dir = ( dir == othDir ? cstDir : othDir );

		player->steps[i].dir = dir;

		//placement
		if (i!=0)
		{
			player->steps[i].coord.x = player->steps[i-1].coord.x + dir2int[player->steps[i-1].dir][0];
			player->steps[i].coord.y = player->steps[i-1].coord.y + dir2int[player->steps[i-1].dir][1];
			player->steps[i].coord.z = player->steps[i-1].coord.z + dir2int[player->steps[i-1].dir][2];
		}
		else
		{
			player->steps[i].coord.x = 0;
			player->steps[i].coord.y = 0;
			player->steps[i].coord.z = 0;
		}
	}
}

void playerRotate ( Player* player, int stepIndex, Snake * snake, int magnet )
{

	if (stepIndex < 0) return;

	int i;
	Dir axe;

	if (stepIndex == 0)
	{
		//pas d'axe de rotation, toutes directions possibles
		
		Dir oldDir = player->steps[0].dir;
		switch ( oldDir )
		{
			case UP		: axe = LEFT; break;	//->front
			case DOWN	: axe = FRONT; break;	//->left
			case LEFT	: axe = DOWN; break;	//->back
			case RIGHT	: axe = FRONT; break;	//->down
			case FRONT	: axe = UP; break;	//->right
			case BACK	: axe = LEFT; break;	//->up
			default		: axe = DNONE; break;	//->say what ?
		}
		player->steps[0].dir = rotationTable[player->steps[0].dir][axe];
		stepIndex++;
		magnet = 1;
	}
	else axe = player->steps[stepIndex-1].dir;

	for ( i = stepIndex; i < snake->length; i++ )
		if ( magnet > 0 )
			player->steps[i].dir = rotationTable[player->steps[i].dir][axe];
		else
			player->steps[i].dir = rotationTable[player->steps[i].dir][(axe%2==0?axe+1:axe-1)];

	for ( i = stepIndex; i < snake->length; i++ )
	{
		player->steps[i].coord.x = player->steps[i-1].coord.x + dir2int[player->steps[i-1].dir][0];
		player->steps[i].coord.y = player->steps[i-1].coord.y + dir2int[player->steps[i-1].dir][1];
		player->steps[i].coord.z = player->steps[i-1].coord.z + dir2int[player->steps[i-1].dir][2];
		mat4x4_translate(player->realCubePos[i],
			(float) player->steps[i].coord.x,
			(float) player->steps[i].coord.y,
			(float) player->steps[i].coord.z);
		mat4x4_identity(player->realCubeRot[i]);
	}

}
void playerFakeRotate ( Player* player, int stepIndex, Snake * snake, int magnet)
{

	if (stepIndex<1) return;

	int i;
	float angle = -(3.1415f*0.5f) * ((float)magnet*0.01f);
	Dir axeDir = player->steps[stepIndex-1].dir;
	vec3 axe;
	for (i=0;i<3;i++) axe[i] = dir2vec[axeDir][i];
	
	for ( i = stepIndex; i < snake->length; i++ )
	{
		mat4x4_identity (player->realCubeRot[i]);
		
		if (axeDir!=0 && axeDir!=1)
			mat4x4_rotate ( player->realCubeRot[i], player->realCubeRot[i], axe[0], axe[1], axe[2], angle );
		else
			mat4x4_rotate ( player->realCubeRot[i], player->realCubeRot[i], axe[0], axe[1], axe[2], -angle );

		if (i!=stepIndex)
		{
			mat4x4_dup ( player->realCubePos[i], player->realCubePos[i-1] );
			mat4x4 tmat;
			mat4x4_identity(tmat);
			if (axeDir!=0 && axeDir!=1)
				mat4x4_rotate ( tmat, tmat, axe[0], axe[1], axe[2], angle );
			else
				mat4x4_rotate ( tmat, tmat, axe[0], axe[1], axe[2], -angle );
			mat4x4_translate_in_place ( tmat, dir2vec[player->steps[i-1].dir][0], 
							dir2vec[player->steps[i-1].dir][1],
							dir2vec[player->steps[i-1].dir][2]);
			if (axeDir!=0 && axeDir!=1)
				mat4x4_rotate ( tmat, tmat, axe[0], axe[1], axe[2], -angle );
			else
				mat4x4_rotate ( tmat, tmat, axe[0], axe[1], axe[2], angle );
			mat4x4_mul (player->realCubePos[i], player->realCubePos[i], tmat);
		}
	}
}

void playerDestroy ( Player* player )
{
	free (player);
}
