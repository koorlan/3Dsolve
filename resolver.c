#include "resolver.h"



void resolverSolveSnake(Snake *snake)
{
    logWrite("[RESO] Starting resolution (snake size : %d x %d x %d)\n",
    snake->volume.max.x, snake->volume.max.y, snake->volume.max.z);
    clock_t startTime = clock();
    int exploredWayNb = 0;

  Tree currentNode = malloc(sizeof(NodeTree));
  Tree rootNode = malloc(sizeof(NodeTree));

  /*Initialize dummy elt */
  printSnake(*snake);
  initTree(&rootNode);

  /* Add all first vectors */
  addInitialVector(&rootNode, 0, 2, 2, RIGHT);
  addInitialVector(&rootNode, 1, 2, 2, LEFT);
  addInitialVector(&rootNode, 1, 1, 2, UP);
  addInitialVector(&rootNode, 1, 2, 2, DOWN);
  addInitialVector(&rootNode, 0, 1, 1, RIGHT);
  addInitialVector(&rootNode, 1, 1, 1, UP);

  currentNode = rootNode->currentChild;

  while (currentNode != rootNode)
  {
    /* solution found */
    if (snake->currentUnit == (snake->length)-1)
    {
      snakeAddStep(snake, &(currentNode->step));
      snakeAddSolution(snake);
      logWrite ("[RESO] Resolver found a solution \n");
      printf ("\n\033[36;01mSolution added\033[00m\n");
      snakeRewind(snake);
      exploredWayNb++;
    }

    /* children already created */
    if(currentNode->hasPlayed)
    {
        if(currentNode->currentChild != NULL)
        {
          currentNode = currentNode->currentChild;
          snake->currentUnit ++;
        }
        else
        {
          if(currentNode->brother == NULL)
          {
              snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FREE;
              currentNode = currentNode->parent;
              free(currentNode->currentChild);
              currentNode->currentChild = NULL;
              snakeRewind(snake);
          }
          else
          {
            snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FREE;
            currentNode = currentNode->brother;
            free(currentNode->parent->currentChild);
            currentNode->parent->currentChild = currentNode;
          }

        }

    }
    else
    {
      if(buildChildren(&currentNode, snake)==1)
        currentNode = currentNode->currentChild;
      else
      {
        exploredWayNb++;
        if(currentNode->brother != NULL)
        {
          snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FREE;
          currentNode = currentNode->brother;
          free(currentNode->parent->currentChild);
          currentNode->parent->currentChild = currentNode;
        }
        else
        {
          snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FREE;
          currentNode = currentNode->parent;
          free(currentNode->currentChild);
          currentNode->currentChild = NULL;
          snakeRewind(snake);
        }
      }
    }
  }
  if (currentNode == rootNode && snake->currentUnit == (snake->length)-1)
  {
    snakeAddSolution(snake);
    printf ("\n\033[36;01mSolution added\033[00m\n");
  }

  clock_t endTime = clock();
  if(endTime == (clock_t)(-1))
    logError("[RESO] CPU clock time not available");

  double elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

  printf("\033[38;01mSnake resolved with\033[00m\033[31;01m %d \033[00m\033[38;01msolution(s) in\033[00m\033[31;01m %f \033[38;01mseconds\033[00m\n", snake->solutions->size,
  elapsedTime);
  printf("\033[31;01m%d \033[00m\033[38;01mways have been explored\033[00m \n", exploredWayNb);
  logWrite ("[RESO] Resolver Ended, found %d solutions in %f seconds\n",snake->solutions->size,
  elapsedTime);

}


void initTree(Tree * rootNode)
{
    Tree elt = malloc(sizeof(NodeTree));

    elt->parent = NULL;
    elt->brother = NULL;
    elt->currentChild = NULL;
    elt->step.coord.x = -1;
    elt->step.coord.y = -1;
    elt->step.coord.z = -1;
    elt->step.dir = DNONE;
    elt->hasPlayed = 1;

    *rootNode = elt;
}

void printSnake(Snake snake)
{
  int i;
  printf ("\n\033[34;01mSnake : \033[00m");
  for (i = 0; i < 27; ++i)
  {
    switch (snake.units[i])
    {   case (EDGE) :
          printf("E");
          break;
        case (STRAIGHT) :
          printf("S");
          break;
        case (CORNER) :
          printf("C");
          break;
    }

  }
  printf("\n\033[34;01mCurrent unit : \033[00m%d", snake.currentUnit);
  printf("\n\n");

}

void copyStep (Step * dest, Step src)
{
  dest->coord.x = src.coord.x;
  dest->coord.y = src.coord.y;
  dest->coord.z = src.coord.z;
  dest->dir = src.dir;

}


void addInitialVector(Tree * rootNode, int x, int y, int z, Dir newDir)
{
  Tree newNode = malloc(sizeof(NodeTree));

  Step newVector;

  newVector.coord.x = x;
  newVector.coord.y = y;
  newVector.coord.z = z;
  newVector.dir = newDir;

  if (newNode == NULL)
  { printf("error of memory allocation\n");
    exit(-1);
  }

  copyStep(&(newNode->step), newVector) ;
  newNode->brother = (*rootNode)->currentChild ;
  newNode->currentChild = NULL;
  newNode->parent = (*rootNode);
  newNode->hasPlayed = 0;
  (*rootNode)->currentChild = newNode ;

}


void printCurrentNode(Tree currentNode)
{
  printf("\033[36;01mVector in current node :\033[00m \n\tCoord  : %d %d %d\n\t", currentNode->step.coord.x, currentNode->step.coord.y, currentNode->step.coord.z);
  switch(currentNode->step.dir)
  {
    case LEFT:
      printf("Arrow : LEFT\n\n");
    break;
    case RIGHT:
      printf("Arrow : RIGHT\n\n");
    break;
    case FRONT:
      printf("Arrow : FRONT\n\n");
    break;
    case BACK:
      printf("Arrow : BACK\n\n");
    break;
    case UP:
      printf("Arrow : UP\n\n");
    break;
    case DOWN:
      printf("Arrow : DOWN\n\n");
    break;
    case DNONE:
      printf("Arrow : DNONE\n\n");
    break;
    default :
      printf("Error in direction value\n\n");
    break;
  }
}


int buildChildren(Tree * currentNode, Snake * snake)
{
  int i = 0;

  Unit nextUnit;

  Tree newChild = malloc(sizeof(NodeTree));

  Coord nCoord = calcCoord((*currentNode)->step.coord, (*currentNode)->step.dir);

  if(validCoord(nCoord, snake->volume.max) && snake->volume.state[nCoord.x][nCoord.y][nCoord.z] == FREE)
  {
      snakeAddStep(snake, &((*currentNode)->step));
      snake->volume.state[(*currentNode)->step.coord.x][(*currentNode)->step.coord.y][(*currentNode)->step.coord.z] = FILL;
      nextUnit = snakeGetNextUnit(snake);
      switch (nextUnit) {
        case EDGE:
        case STRAIGHT:
          newChild->parent = *currentNode;
          newChild->brother = (*currentNode)->currentChild;
          newChild->hasPlayed = 0;
          newChild->currentChild = NULL;
          newChild->step.dir = (*currentNode)->step.dir;
          newChild->step.coord.x = nCoord.x;
          newChild->step.coord.y = nCoord.y;
          newChild->step.coord.z = nCoord.z;
          (*currentNode)->currentChild = newChild;
          break;
        case CORNER:
          printf("");
          free(newChild);

          for ( i = 0; i < 6; i++)
          {
            Tree newChild = malloc(sizeof(NodeTree));
            if(cornerTruthTable[(*currentNode)->step.dir][i]){
              Dir tempDir;
              switch(i){
                case 0:
                  tempDir = UP ;
                  break;
                case 1:
                  tempDir = DOWN ;
                  break;
                case 2:
                  tempDir = LEFT ;
                  break;
                case 3:
                  tempDir = RIGHT ;
                  break;
                case 4:
                  tempDir = FRONT ;
                  break;
                case 5:
                  tempDir = BACK ;
                  break;
              }


              newChild->parent = *currentNode;
              newChild->brother = (*currentNode)->currentChild;
              newChild->hasPlayed = 0;
              newChild->currentChild = NULL;

              newChild->step.dir = tempDir;
              newChild->step.coord.x = nCoord.x;
              newChild->step.coord.y = nCoord.y;
              newChild->step.coord.z = nCoord.z;

              (*currentNode)->currentChild = newChild;
              }
          }
          break;
      }
      (*currentNode)->hasPlayed = 1;
    return 1;
  }
  else
    return -1;
}


void printTree (Tree rootNode)
{
  Tree tmpNode = malloc(sizeof(NodeTree));
  tmpNode=rootNode->currentChild;
  while(tmpNode!=NULL)
  {
    printCurrentNode(tmpNode);
    tmpNode=tmpNode->brother;
  }
}


void resolverFindSymmetry(Volume volume)
{
  int i,j,k,l ;
  int nbCube = volume.max.x * volume.max.y * volume.max.z;
  Coord cubeCenter;
  cubeCenter.x = volume.max.x/2;
  cubeCenter.y = volume.max.y/2;
  cubeCenter.x = volume.max.z/2;

  Step ****figure = malloc( volume.max.x * sizeof(*figure));
  for ( i = 0; i < volume.max.x; i++)
  {
    figure[i] = malloc( volume.max.y * sizeof(**figure));
  }
  for ( i = 0; i < volume.max.x; i++)
  {
    for ( j = 0; j < volume.max.y; j++)
    {
      figure[i][j] = malloc( volume.max.z * sizeof(***figure));
    }
  }
  for ( i = 0; i < volume.max.x; i++)
  {
    for ( j = 0; j < volume.max.y; j++)
    {
      for ( k = 0; k < volume.max.y; k++)
      {
        figure[i][j][k] = malloc(6 * sizeof(****figure));
        for ( l = 0; l < 6; l++)
        {
              figure[i][j][k][l].dir = l ;

        }
      }
    }
  }


  for ( i = 0; i < volume.max.x; i++)
  {
    for ( j = 0; j < volume.max.y; j++)
    {
      for ( k = 0; k < volume.max.y; k++)
      {
          free(figure[i][j][k]);
        }
      }
    }

  for ( i = 0; i < volume.max.x; i++)
  {
    for ( j = 0; j < volume.max.y; j++)
    {
      figure[i][j] = malloc( volume.max.z * sizeof(***figure));
    }
  }

  for ( i = 0; i < volume.max.x; i++)
  {
    free(figure[i]);
  }

  free(figure);


}

Coord calcCoord(Coord coord,Dir dir){
  Coord nCoord;
  switch (dir){
    case UP:
      nCoord.x = coord.x + 0;
      nCoord.y = coord.y + 1;
      nCoord.z = coord.z + 0;
      break;
    case DOWN:
      nCoord.x = coord.x + 0;
      nCoord.y = coord.y - 1;
      nCoord.z = coord.z + 0;
      break;
    case LEFT:
      nCoord.x = coord.x - 1;
      nCoord.y = coord.y + 0;
      nCoord.z = coord.z + 0;
      break;
    case RIGHT:
      nCoord.x = coord.x + 1;
      nCoord.y = coord.y + 0;
      nCoord.z = coord.z + 0;
      break;
    case FRONT:
      nCoord.x = coord.x + 0;
      nCoord.y = coord.y + 0;
      nCoord.z = coord.z - 1;
      break;
    case BACK:
      nCoord.x = coord.x + 0;
      nCoord.y = coord.y + 0;
      nCoord.z = coord.z + 1;
      break;
    default :
      printf("Error in direction value\n");
      break;
  }
  return nCoord;
}

int validCoord(Coord coord, Coord max)
{
  return (coord.x >= 0 && coord.x < max.x) && (coord.y >= 0 && coord.y < max.y) && (coord.z >= 0 && coord.z < max.z) ;
}
