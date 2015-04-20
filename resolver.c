#include "resolver.h"

void resolverSolveSnake(Snake *snake)
{
  NodeTree *currentNode = NULL;
  NodeTree *rootNode = NULL;
  /*Initialization*/
  rootNode = malloc(sizeof(NodeTree));
  rootNode->parent = NULL;
  rootNode->brother = NULL;
  rootNode->hasPlayed = 0;
  rootNode->currentChild = NULL;

  rootNode->step.coord.x = 2;
  rootNode->step.coord.y = 0;
  rootNode->step.coord.z = 2;
  rootNode->step.dir = UP;

  Coord nCoord;
  int i,firstChild = 1;
  NodeTree *tempChild = NULL;
  nCoord = calcCoord(rootNode->step.coord, rootNode->step.dir);
  if(validCoord(nCoord,snake->volume.max) && snake->volume.state[nCoord.x][nCoord.y][nCoord.z] == FREE)
  {
    snakeAddStep(snake,&rootNode->step);
    snake->volume.state[rootNode->step.coord.x][rootNode->step.coord.y][rootNode->step.coord.z] = BUSY;
    Unit nextUnit = snakeGetNextUnit(snake);

    switch (nextUnit) {
      case EDGE:
      case STRAIGHT:
        printf("");
        //For the moment ... same action..
        NodeTree *newChild = malloc( sizeof (NodeTree));
        newChild->parent = rootNode;
        newChild->brother = NULL;
        newChild->hasPlayed = 0;
        newChild->currentChild = NULL;
        newChild->step.dir = rootNode->step.dir;
        newChild->step.coord.x = nCoord.x;
        newChild->step.coord.y = nCoord.y;
        newChild->step.coord.z = nCoord.z;
        rootNode->currentChild = newChild;
        break;
      case CORNER:
        for ( i = 0; i < 6; i++)
        {
          if(cornerTruthTable[rootNode->step.dir][i]){
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
            NodeTree *newChild = malloc( sizeof (NodeTree));
            newChild->parent = rootNode;
            newChild->brother = NULL;
            newChild->hasPlayed = 0;
            newChild->currentChild = NULL;

            newChild->step.dir = tempDir;
            newChild->step.coord.x = nCoord.x;
            newChild->step.coord.y = nCoord.y;
            newChild->step.coord.z = nCoord.z;

            if(firstChild)
            {
              rootNode->currentChild = newChild;
              firstChild = 0;
            }

            if(tempChild != NULL)
            {
              tempChild->brother = newChild;
            }

            tempChild = newChild;

          }
        }
        break;
    }
    rootNode->hasPlayed = 1;
    currentNode = rootNode->currentChild;
  }
  else
  {
    return; //Impossible snake ...
  }

  while(currentNode != rootNode && rootNode->hasPlayed == 1 && !(rootNode->currentChild == NULL))
  {

    if (snake->currentUnit == snake->length -1)
    {
    snakeAddSolution(snake);
    printf("Added a solution\n");
    snakeRewind(snake);
    }

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
            //snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FREE;
            currentNode = currentNode->brother;
            free(currentNode->parent->currentChild);
            currentNode->parent->currentChild = currentNode;
            //snakeRewind(snake);
          }
        }
      continue;
    }
    Coord nCoord;
    int i,firstChild = 1;
    NodeTree *tempChild = NULL;
    nCoord = calcCoord(currentNode->step.coord, currentNode->step.dir);
    if(validCoord(nCoord,snake->volume.max) && snake->volume.state[nCoord.x][nCoord.y][nCoord.z] == FREE)
    {
      snakeAddStep(snake,&currentNode->step);
      snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = BUSY;
      Unit nextUnit = snakeGetNextUnit(snake);
      NodeTree *newChild = malloc( sizeof (NodeTree));
      switch (nextUnit) {
        case EDGE:
        case STRAIGHT:
          //For the moment ... same action..
          newChild->parent = currentNode;
          newChild->brother = NULL;
          newChild->hasPlayed = 0;
          newChild->currentChild = NULL;
          newChild->step.dir = currentNode->step.dir;
          newChild->step.coord.x = nCoord.x;
          newChild->step.coord.y = nCoord.y;
          newChild->step.coord.z = nCoord.z;
          currentNode->currentChild = newChild;
          break;
        case CORNER:
          free(newChild); //very sale..
          for ( i = 0; i < 6; i++)
          {
            if(cornerTruthTable[currentNode->step.dir][i]){
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

              NodeTree *newChild = malloc( sizeof (NodeTree));
              newChild->parent = currentNode;
              newChild->brother = NULL;
              newChild->hasPlayed = 0;
              newChild->currentChild = NULL;

              newChild->step.dir = tempDir;
              newChild->step.coord.x = nCoord.x;
              newChild->step.coord.y = nCoord.y;
              newChild->step.coord.z = nCoord.z;

              if(firstChild)
              {
                currentNode->currentChild = newChild;
                firstChild = 0;
              }

              if(tempChild != NULL)
              {
                tempChild->brother = newChild;
              }

              tempChild = newChild;

            }
          }
          break;
      }
      currentNode->hasPlayed = 1;
      currentNode = currentNode->currentChild;

    }
    else
    {
      if(currentNode->brother != NULL)
      {
        currentNode = currentNode->brother;
        free(currentNode->parent->currentChild);
        currentNode->parent->currentChild = currentNode;
        //snakeRewind(snake);
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
  }
  return nCoord;
}

int validCoord(Coord coord, Coord max){
  return (coord.x >= 0 && coord.x < max.x) && (coord.y >= 0 && coord.y < max.y) && (coord.z >= 0 && coord.z < max.z) ;
}
