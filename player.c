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
	if (playerCheckSolution(gplayer, app->snake->volume, app->snake->length)==1)
		bhv_flags |= BHV_ROTATE;
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
	if (player==NULL) return;
	free(player->steps);
	free(player->realCubePos);
	free(player->realCubeRot);
	free(player->flatCubePos);
	free (player);
}


int playerFindMinMax (Coord *min, Coord *max, int length, Player * player, Volume volume)
{	int i, j;

	for(i=0;i<length;i++)
	{
		for(j=0; j<i ; j++)
		{
			if(player->steps[i].coord.x == player->steps[j].coord.x && player->steps[i].coord.y == player->steps[j].coord.y
			 && player->steps[i].coord.z == player->steps[j].coord.z)
			{	
				//printf("\033[31;01mPlayer didn't find the solution - cubes se chevauchent\033[00m\n");
				return -1;
			}

		}
		if(player->steps[i].coord.x < min->x)
		{
			min->x = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y < min->y)
		{
			min->y = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z < min->z)
		{
			min->z = player->steps[i].coord.z;
		}
		if(player->steps[i].coord.x > max->x)
		{
			max->x = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y > max->y)
		{
			max->y = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z > max->z)
		{
			max->z = player->steps[i].coord.z;
		}
	}
	if(max->x-min->x > volume.max.x || max->y-min->y > volume.max.y || max->z-min->z > volume.max.z)
	{	
		//printf("\033[31;01mPlayer didn't find the solution - sort de la structure \033[00m\n");
		return -1;
	}
	min->x = -(min->x);

	min->y = -(min->y);

	min->z = -(min->z);
	return 1;
}

int playerCheckSolution (Player *player, Volume volume, int length)
{
	Coord min, max;
	min.x = 100;
	min.y = 100;
	min.z = 100;
	max.x = -100;
	max.y = -100;
	max.z = -100;
	int i;

	if(playerFindMinMax(&min, &max, length, player, volume) == -1)
		return -1;

	for(i=0;i<length;i++)
	{	
		if(!(player->steps[i].coord.x + min.x < volume.max.x && player->steps[i].coord.y + min.y < volume.max.y && player->steps[i].coord.z + min.z < volume.max.z && volume.state[player->steps[i].coord.x + min.x][player->steps[i].coord.y + min.y][player->steps[i].coord.z + min.z] == FREE))
		{	
			//printf("\033[31;01mPlayer didn't find the solution\033[00m\n");
			return -1;
		}
	}	
	printf("\033[36;01mPlayer found the solution\033[00m\n");
	return 1;
}

int initVolume(Snake *solSnake, Snake *snake, Player * player, Coord min, Coord max, int *debugCpt)
{	int x, y, z, i;

	for(x=0; x < solSnake->volume.max.x; x++)
	{	for(y=0; y < solSnake->volume.max.y ; y++)
		{	for(z=0; z < solSnake->volume.max.z ; z++)
			{
				solSnake->volume.state[x][y][z] = snake->volume.state[x][y][z];
			}
		}
	}
	for(i=0;i<player->selected + 1;i++)
	{	
		if(!(player->steps[i].coord.x + min.x < solSnake->volume.max.x && 
			player->steps[i].coord.y + min.y < solSnake->volume.max.y && 
			player->steps[i].coord.z + min.z < solSnake->volume.max.z && 
			solSnake->volume.state[player->steps[i].coord.x + min.x][player->steps[i].coord.y + min.y][player->steps[i].coord.z + min.z] == FREE))
		{	printf("\033[31;01mPlayer didn't find the solution - en dehors du vol ou chevauchement\033[00m\n");
			
			return -1;
		}
		else if(player->steps[i].coord.x + min.x < solSnake->volume.max.x && 
			player->steps[i].coord.y + min.y < solSnake->volume.max.y && 
			player->steps[i].coord.z + min.z < solSnake->volume.max.z &&
			solSnake->volume.state[player->steps[i].coord.x + min.x][player->steps[i].coord.y + min.y][player->steps[i].coord.z + min.z] == FREE)
		{	
			solSnake->volume.state[player->steps[i].coord.x + min.x][player->steps[i].coord.y + min.y][player->steps[i].coord.z + min.z] = FILL;
			(*debugCpt) ++;
		}
	}
	return 1;
}

int playerHelp(Player *player, Snake * snake)
{

	int i, j, k;
	int debugCpt = 0;
	Coord min, max;
	Step fstStep;

	min.x = 100;
	min.y = 100;
	min.z = 100;
	max.x = -100;
	max.y = -100;
	max.z = -100;

	Snake * solSnake = malloc(sizeof(Snake));
	if(solSnake == NULL)
	{
		logError("[PHELP] Error on M alloc\n");
		exit(-1);
	}
	//if(player->selected ==0)
	//	return -1;
	solSnake->length = snake->length - player->selected - 1; 
	solSnake->tmpSteps = NULL;
	solSnake->units = NULL;
	solSnake->solutions = NULL; 
	solSnake->volume.state = NULL;
	
	/* initialize a new volume */
	solSnake->volume.max.x = snake->volume.max.x;
	solSnake->volume.max.y = snake->volume.max.y;
	solSnake->volume.max.z = snake->volume.max.z;
	solSnake->volume.state = malloc(solSnake->volume.max.x * sizeof(VolumeState*));

	for(i = 0; i < solSnake->volume.max.x; i++)
	{	solSnake->volume.state[i] = malloc(solSnake->volume.max.y * sizeof(VolumeState));
		if(solSnake->volume.state[i] == NULL)
		{
			logError("[PHELP] Error on M alloc\n");
			exit(-1);
		}
	}


	for(i = 0; i < solSnake->volume.max.x; i++)
	{	for(j = 0; j < solSnake->volume.max.y; j++)
		{	
			solSnake->volume.state[i][j] = malloc(solSnake->volume.max.z * sizeof(VolumeState));
			if(solSnake->volume.state[i][j] == NULL)
			{
				logError("[PHELP] Error on M alloc\n");
				exit(-1);
			}
		}
	}
	
	if(playerFindMinMax(&min, &max, player->selected + 1, player, snake->volume) == -1)
	{	
		return -1; 
	}

	/* Initialize a new string of units */
	if(solSnake->length > 0)
	{	solSnake->units = malloc(solSnake->length * sizeof(Unit));
		if(solSnake->units == NULL)
		{
			logError("[PHELP] Error on M alloc\n");
			exit(-1);
		}
	}
	else 
	{	
		return -1; 
	}
	for(i=0; i< solSnake->length; i++)
		solSnake->units[i] = snake->units[i+player->selected+1];

	/*** ready to go now ***/
	solSnake->currentUnit = 0;
	solSnake->solutions = listSolutionCreate();
	solSnake->tmpSteps = malloc(solSnake->length * sizeof(Step));
	memset(solSnake->tmpSteps, 0,solSnake->length * sizeof(Step));

	
	int dx, ddx, ddy, ddz, dy, dz;
	dx = solSnake->volume.max.x-(max.x+min.x) -1;
	dy = solSnake->volume.max.y-(max.y+min.y) -1;
	dz = solSnake->volume.max.z-(max.z+min.z) -1;
	ddx = min.x;
	ddy = min.y;
	ddz = min.z;
	if(dx<0 || dx > solSnake->volume.max.x-1 || dy<0 || dy > solSnake->volume.max.y-1 ||dz<0 || dz > solSnake->volume.max.z-1)
	{
		logError ("[PHELP] Not allowed value for (dx dy dz)\n");
		return -1;
	}
	for(i=0; i<=dx; i++)
	{
		for(j=0;j<=dy; j++)
		{
			for(k=0;k<=dz; k++)
			{	
				min.x = ddx + i;
				min.y = ddy + j;
				min.z = ddz + k;
				
				fstStep.coord.x = player->steps[player->selected].coord.x + min.x;
				fstStep.coord.y = player->steps[player->selected].coord.y + min.y;
				fstStep.coord.z = player->steps[player->selected].coord.z + min.z;
				fstStep.dir = player->steps[player->selected].dir;
				solSnake->currentUnit = 0;
				debugCpt = 0;
				initVolume(solSnake, snake, player, min, max, &debugCpt);
				logWrite("[PHELP] solSnake -> length = %d \nNb of FILL units in the volume %d\n", solSnake->length, debugCpt);
				resolverInitializeHelp(solSnake, fstStep);
				if(solSnake->solutions->head != NULL)
				{	
					i=0;
					solSnake->solutions->head->step[i].coord.x -= min.x;
					solSnake->solutions->head->step[i].coord.y -= min.y;
					solSnake->solutions->head->step[i].coord.z -= min.z;
					copyStep(&(player->steps[player->selected+1]), solSnake->solutions->head->step[i]);
					player->selected++;
					if (solSnake->units[i]==CORNER) return 1;
					i++;
					while(solSnake->units[i-1] != CORNER && i < solSnake->length)
					{	
						//memcpy(&(solSnake->solutions->head->step[i].coord), translateCoord(solSnake->solutions->head->step[i], min, -1), sizeof(Coord));
						solSnake->solutions->head->step[i].coord.x -= min.x;
						solSnake->solutions->head->step[i].coord.y -= min.y;
						solSnake->solutions->head->step[i].coord.z -= min.z;
						copyStep(&(player->steps[player->selected+1]), solSnake->solutions->head->step[i]);
						player->selected++;
						i++;
						
					}
					return 1;	
				}	
			}
		}
	}	
	return -1; 
} 

