#include "player.h"

//UP,DOWN,LEFT,RIGHT,FRONT,BACK
//Axes en deuxième position
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


Player* playerInit ( Snake* snake )
{
	Player* player = malloc ( sizeof (Player) );
	player->steps = malloc ( snake->length * sizeof(Step) );
	player->selected = -1;
	player->steps[0].dir = RIGHT;
	playerFlatten ( player, snake, 0 );
	player->currentSolution = NULL;
	
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

void playerRotate ( Player* player, int stepIndex, Snake * snake, int way )
{

	if (stepIndex<1) return;

	Dir axe = player->steps[stepIndex-1].dir;
	int i;
	for ( i = stepIndex; i < snake->length; i++ )
		if (way == 0)
			player->steps[i].dir = rotationTable[player->steps[i].dir][axe];
		else
			player->steps[i].dir = rotationTable[player->steps[i].dir][(axe%2==0?axe+1:axe-1)];
		

	for ( i = stepIndex; i < snake->length; i++ )
	{
		player->steps[i].coord.x = player->steps[i-1].coord.x + dir2int[player->steps[i-1].dir][0];
		player->steps[i].coord.y = player->steps[i-1].coord.y + dir2int[player->steps[i-1].dir][1];
		player->steps[i].coord.z = player->steps[i-1].coord.z + dir2int[player->steps[i-1].dir][2];
	}

}

void playerDestroy ( Player* player )
{
	free (player);
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
	int i, j;
	for(i=0;i<length;i++)
	{
		for(j=0; j<i ; j++)
		{
			if(player->steps[i].coord.x == player->steps[j].coord.x && player->steps[i].coord.y == player->steps[j].coord.y && player->steps[i].coord.z == player->steps[j].coord.z)
				return -1;
		}
		if(player->steps[i].coord.x < minX)
		{
			minX = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y < minY)
		{
			minY = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z < minZ)
		{
			minZ = player->steps[i].coord.z;
		}
		if(player->steps[i].coord.x > maxX)
		{
			maxX = player->steps[i].coord.x;
		}
		if(player->steps[i].coord.y > maxY)
		{
			maxY = player->steps[i].coord.y;
		}
		if(player->steps[i].coord.z > maxZ)
		{
			maxZ = player->steps[i].coord.z;
		}
	}
	if(maxX-minX > volume.max.x || maxY-minY > volume.max.y || maxZ-minZ > volume.max.z)
		return -1;
	minX = -minX;

	minY = -minY;

	minZ = -minZ;

	for(i=0;i<length;i++)
	{	
		if(!(player->steps[i].coord.x + minX < volume.max.x && player->steps[i].coord.y + minY < volume.max.y && player->steps[i].coord.z + minZ < volume.max.z && volume.state[player->steps[i].coord.x + minX][player->steps[i].coord.y + minY][player->steps[i].coord.z + minZ] == FREE))
			return -1;
	}	
	printf("\033[36;01mPlayer found the solution\033[00m\n");
	return 1;
}