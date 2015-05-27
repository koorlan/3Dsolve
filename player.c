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


	int i;
	//for ( i = 0; i < snake->length; i++ )
	for ( i = fromIndex; i < snake->length; i++ )
	{
		//direction
		if ( snake->units[i] == CORNER )
		{
			dir = (dir==BACK?RIGHT:BACK);
			//dir = rotationTable[(dir+2)%6][dir];
		}
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

	if (stepIndex<1) return;
	Dir axe = player->steps[stepIndex-1].dir;
	int i;
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


int playerFindMinMax (int * minX, int* minY, int* minZ, int *maxX, int *maxY, int* maxZ, int length, Player * player, Volume volume)
{	int i, j;

	for(i=0;i<length;i++)
	{
		for(j=0; j<i ; j++)
		{
			if(player->steps[i].coord.x == player->steps[j].coord.x && player->steps[i].coord.y == player->steps[j].coord.y && player->steps[i].coord.z == player->steps[j].coord.z)
			{	printf("\033[31;01mPlayer didn't find the solution\033[00m\n");
				return -1;
			}

		}
		if(player->steps[i].coord.x < *minX)
		{
			*minX = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y < *minY)
		{
			*minY = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z < *minZ)
		{
			*minZ = player->steps[i].coord.z;
		}
		if(player->steps[i].coord.x > *maxX)
		{
			*maxX = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y > *maxY)
		{
			*maxY = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z > *maxZ)
		{
			*maxZ = player->steps[i].coord.z;
		}
	}
	if(*maxX-*minX > volume.max.x || *maxY-*minY > volume.max.y || *maxZ-*minZ > volume.max.z)
	{	printf("\033[31;01mPlayer didn't find the solution\033[00m\n");
		return -1;
	}
	return 0;
}

int playerCheckSolution (Player *player, Volume volume, int length)
{
	int minX, minY, minZ, maxX, maxY, maxZ;
	minX = 100;
	minY = 100;
	minZ = 100;
	maxX = -100;
	maxY = -100;
	maxZ = -100;
	int i;

	if(playerFindMinMax(&minX, &minY, &minZ, &maxX, &maxY, &maxZ, length, player, volume) == -1)
		return -1;

	minX = -minX;

	minY = -minY;

	minZ = -minZ;

	for(i=0;i<length;i++)
	{	
		if(!(player->steps[i].coord.x + minX < volume.max.x && player->steps[i].coord.y + minY < volume.max.y && player->steps[i].coord.z + minZ < volume.max.z && volume.state[player->steps[i].coord.x + minX][player->steps[i].coord.y + minY][player->steps[i].coord.z + minZ] == FREE))
		{	printf("\033[31;01mPlayer didn't find the solution\033[00m\n");
			return -1;
		}
	}	
	printf("\033[36;01mPlayer found the solution\033[00m\n");
	return 1;
}

void playerHelp(Player *player, Snake * snake)
{
	Snake * solSnake = malloc(sizeof(Snake));
	int i, j, x, y, z;

	solSnake->length = snake->length - player->selected; 
	solSnake->tmpSteps = NULL;
	solSnake->units = NULL;
	solSnake->solutions = NULL;
	solSnake->volume.state = NULL;
	
	solSnake->volume.max.x = snake->volume.max.x;
	solSnake->volume.max.y = snake->volume.max.y;
	solSnake->volume.max.z = snake->volume.max.z;

	for(i = 0; i < solSnake->volume.max.x; i++)
		solSnake->volume.state[i] = malloc(solSnake->volume.max.y * sizeof(VolumeState*));
	for(i = 0; i < solSnake->volume.max.x; i++)
		for(j = 0; j < solSnake->volume.max.y; j++)
			solSnake->volume.state[i][j] = malloc(solSnake->volume.max.z * sizeof(VolumeState));

	for(x=0; x < solSnake->volume.max.x; x++)
	{	for(y=0; y< solSnake->volume.max.y ; y++)
		{	for(z=0; z<solSnake->volume.max.z ; z++)
			{
				solSnake->volume.state[x][y][z] = snake->volume.state[x][y][z];
			}
		}
	}


}