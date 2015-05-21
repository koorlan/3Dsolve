#include "player.h"

//UP,DOWN,LEFT,RIGHT,FRONT,BACK
//AXE IN SECOND POSITION
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

	playerFlatten ( player, snake, 0 );
	
	return player;
}

void playerFlatten ( Player* player, Snake* snake, int fromIndex )
{

	Dir dir;
	if (fromIndex==0) dir = RIGHT;
	else dir = player->steps[fromIndex].dir;


	int i;
	for ( i = fromIndex; i < snake->length; i++ )
	{
		player->steps[i].dir = dir;
		if (i!=0)
		{
			player->steps[i].coord.x = player->steps[i-1].coord.x + (dir==RIGHT?1:0);
			player->steps[i].coord.y = 0;
			player->steps[i].coord.z = player->steps[i-1].coord.z - (dir==BACK?1:0);
		}
		else
		{
			player->steps[i].coord.x = 0;
			player->steps[i].coord.y = 0;
			player->steps[i].coord.z = 0;
		}

		if ( snake->units[i] == CORNER)
		{
			if ( dir == BACK ) dir = RIGHT;
			else dir = BACK;
		}
	}
}

void playerRotate ( Player* player, int stepIndex, Snake * snake)
{
	if (stepIndex<1) return;

	Dir axe = player->steps[stepIndex-1].dir;
	int i;
	for ( i = stepIndex; i < snake->length; i++ )
		player->steps[i].dir = rotationTable[player->steps[i].dir][axe];

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
