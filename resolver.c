#include "resolver.h"

const int cornerTruthTable[6][6] =
 {
   {0 , 0 , 1 , 1 , 1 , 1 },
   {0 , 0 , 1 , 1 , 1 , 1 },
   {1 , 1 , 0 , 0 , 1 , 1 },
   {1 , 1 , 0 , 0 , 1 , 1 },
   {1 , 1 , 1 , 1 , 0 , 0 },
   {1 , 1 , 1 , 1 , 0 , 0 }
 } ;

void resolverSolveSnake(Snake *snake)
{
  Tree currentNode = malloc(sizeof(NodeTree));
  Tree rootNode = malloc(sizeof(NodeTree));

  rootNode = findInitialVectors(snake->volume);

  printSnake(*snake);

  printTree(rootNode);

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

  logWrite ("[RESO] Resolver Ended, found %d solutions\n",snake->solutions->size);

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
  for (i = 0; i < snake.length; ++i)
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
  printf("\033[36;01mVector in current node :\033[00m \n\tCoord  : %d %d %d\n\t", \
    currentNode->step.coord.x, currentNode->step.coord.y, currentNode->step.coord.z);

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

void printLine (Line line)
{
  printf("A : (%lf %lf, %lf)\tB : (%lf, %lf, %lf)\nd : %lfx + %lfy = %lf\n", \
  line.pointA.x, line.pointA.y, line.pointA.z, \
  line.pointB.x, line.pointB.y, line.pointB.z, \
  line.a, line.b, line.c );
}

void linearEquation (Line *line)
{

  if((line->pointB.x - line->pointA.x)==0)
  { line->a = 1;
    line->b = 0;
  }
  else
  {
    line->a = (int) (line->pointB.y - line->pointA.y) / (line->pointB.x - line->pointA.x);
    line->b = -1;
  }
  line->c = line->a * line->pointA.x - line->pointA.y;
}

int oppositeDir(Dir srcDir, Dir destDir, char * typeOfAxis)
{
  if(!strcmp(typeOfAxis, "vertical"))
    return ((srcDir==UP && destDir==UP) || (srcDir==DOWN && destDir==DOWN) || (srcDir==LEFT && destDir==RIGHT) || (srcDir==RIGHT && destDir==LEFT));
  else if(!strcmp(typeOfAxis, "horizontal"))
    return ((srcDir==UP && destDir==DOWN) || (srcDir==DOWN && destDir==UP) || (srcDir==LEFT && destDir==LEFT) || (srcDir==RIGHT && destDir==RIGHT));
  else if(!strcmp(typeOfAxis, "diagonal"))
    return((srcDir==UP && destDir==LEFT) || (srcDir==LEFT && destDir==UP) || (srcDir==RIGHT && destDir==DOWN) || (srcDir==DOWN && destDir==RIGHT));
  else
    return((srcDir==UP && destDir==RIGHT) || (srcDir==RIGHT && destDir==UP) || (srcDir==LEFT && destDir==DOWN) || (srcDir==DOWN && destDir==LEFT));

}

void cpyLine(Line *dest, Line src)
{
  dest->a = src.a;
  dest->b = src.b;
  dest->c = src.c;
}

int symmetries (Step initialStep, Coord nCoord, Dir nDir, Line verticalAxis, Line horizontalAxis, Line diagonalAxis, Line slashAxis)
{
  Line symmetryAxis;
  char *typeOfAxis;
  int i;

  for(i=0; i<4; i++)
  {
    switch (i)
    {
      case 0 :
        cpyLine(&symmetryAxis, verticalAxis);
        typeOfAxis = "vertical";
        break;
      case 1 :
        cpyLine(&symmetryAxis, horizontalAxis);
        typeOfAxis = "horizontal";
        break;
      case 2 :
        cpyLine(&symmetryAxis, diagonalAxis);
        typeOfAxis = "diagonal";
        break;
      case 3 :
        cpyLine(&symmetryAxis, slashAxis);
        typeOfAxis = "slash";
        break;

    }

    float n = symmetryAxis.a*symmetryAxis.a + symmetryAxis.b*symmetryAxis.b;
    int cm = symmetryAxis.a*initialStep.coord.x + symmetryAxis.b*initialStep.coord.y;
    int xm = initialStep.coord.x + 2*symmetryAxis.a*(symmetryAxis.c - cm)/n;
    int ym = initialStep.coord.y + 2*symmetryAxis.b*(symmetryAxis.c - cm)/n;

    if(nCoord.x == xm && nCoord.y == ym && oppositeDir(initialStep.dir, nDir, typeOfAxis))
    {
      printf ("\n\033[31;01mSymmetry found here\033[00m\n");
      printf("%d %d %d ", nCoord.x, nCoord.y, nCoord.z);
      switch(nDir)
      {
        case UP :
          printf("UP\n");
          break;
        case DOWN :
          printf("DOWN\n");
          break;
        case LEFT :
          printf("LEFT\n");
          break;
        case RIGHT :
          printf("RIGHT\n");
          break;
        default :
          printf("error\n");
          break;
      }
      return 1;
    }
  }
  return 0;
}

int lineCmp(Step step, Coord coord, Dir dir)
{
  return(step.coord.x == coord.x && step.coord.y == coord.y && step.coord.z == coord.z && dir == step.dir);
}

Tree findInitialVectors(Volume volume)
{
  FloatCoord projectionCenter;

  Line verticalAxis;
  Line horizontalAxis;
  Line diagonalAxis;
  Line slashAxis;

  int i, j, k, dirIndex, flag;

  Tree initialNode = malloc(sizeof(NodeTree));

  Coord nCoord;

  FloatCoord cubeCenter;
  cubeCenter.x = (float)(volume.max.x-1)/2;
  cubeCenter.y = (float)(volume.max.y-1)/2;
  cubeCenter.z = (float)(volume.max.z-1)/2;

  printf("cube center %lf %lf %lf\n", cubeCenter.x, cubeCenter.y, cubeCenter.z);

  Tree tmpNode = malloc(sizeof(NodeTree));

  initTree(&initialNode);
  addInitialVector(&initialNode, 0, 0, 2, UP);
  tmpNode=initialNode->currentChild;

  k = 0;
  flag = 0;

  //select each face of the cube, parallel to the plan (0,x,y)
  while(volume.max.z -1 - k > 0)

  {
    //projected othogonal of the cube center on the considered face
    projectionCenter.x = cubeCenter.x;
    projectionCenter.y = cubeCenter.y;
    projectionCenter.z = volume.max.z -1 - k;

    //initialize axes of symmetry
    verticalAxis.pointA.x = projectionCenter.x;
    verticalAxis.pointA.y = projectionCenter.y-1;
    verticalAxis.pointA.z = projectionCenter.z;
    verticalAxis.pointB.x = projectionCenter.x;
    verticalAxis.pointB.y = projectionCenter.y+1;
    verticalAxis.pointB.z = projectionCenter.z;

    horizontalAxis.pointA.x = projectionCenter.x-1;
    horizontalAxis.pointA.y = projectionCenter.y;
    horizontalAxis.pointA.z = projectionCenter.z;
    horizontalAxis.pointB.x = projectionCenter.x+1;
    horizontalAxis.pointB.y = projectionCenter.y;
    horizontalAxis.pointB.z = projectionCenter.z;

    diagonalAxis.pointA.x = projectionCenter.x-1;
    diagonalAxis.pointA.y = projectionCenter.y+1;
    diagonalAxis.pointA.z = projectionCenter.z;
    diagonalAxis.pointB.x = projectionCenter.x+1;
    diagonalAxis.pointB.y = projectionCenter.y-1;
    diagonalAxis.pointB.z = projectionCenter.z;

    slashAxis.pointA.x = projectionCenter.x+1;
    slashAxis.pointA.y = projectionCenter.y+1;
    slashAxis.pointA.z = projectionCenter.z;
    slashAxis.pointB.x = projectionCenter.x-1;
    slashAxis.pointB.y = projectionCenter.y-1;
    slashAxis.pointB.z = projectionCenter.z;


    //calculation of the linear equations
    linearEquation(&verticalAxis);
    linearEquation(&horizontalAxis);
    linearEquation(&diagonalAxis);
    linearEquation(&slashAxis);

    printf ("\n\033[38;01mVertical axis\033[00m\n");
    printLine(verticalAxis);
    printf ("\n\033[38;01mHorizontal axis\033[00m\n");
    printLine(horizontalAxis);
    printf ("\n\033[38;01mDiagonal axis\033[00m\n");
    printLine(diagonalAxis);
    printf ("\n\033[38;01mSlash axis\033[00m\n");
    printLine(slashAxis);

    for(i=0; i<=volume.max.x/2; i++)
    {
      for(j=0; j<=volume.max.y/2; j++)
      {
        tmpNode=initialNode->currentChild;
        nCoord.x = i;
        nCoord.y = j;
        nCoord.z = projectionCenter.z;

        for (dirIndex = 0; dirIndex < 4; dirIndex++)
        {
          tmpNode=initialNode->currentChild;
          Dir tempDir;
          switch(dirIndex)
          {
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
          }
          if(validCoordSym(calcCoord(nCoord, tempDir), projectionCenter))
          {
            while(tmpNode!=NULL && !flag )
            {
              if(!lineCmp(tmpNode->step, nCoord, tempDir))
                flag = symmetries(tmpNode->step, nCoord, tempDir, verticalAxis, horizontalAxis, diagonalAxis, slashAxis);
              else
                flag = 1;
              tmpNode=tmpNode->brother;
            }
            if(flag)
            {
              printf("symmetry found\n");
              flag = 0;

            }
            else
            { printf("symmetry not found\n");
              addInitialVector(&initialNode, i, j, projectionCenter.z, tempDir);
            }
          }
          else
          { printf ("\n\033[31;01mUnvalid coord\033[00m\n");
            printf("%d %d %d ", nCoord.x, nCoord.y, nCoord.z);
            switch(tempDir)
            {
                case UP :
                  printf("UP\n");
                  break;
                case DOWN :
                  printf("DOWN\n");
                  break;
                case LEFT :
                  printf("LEFT\n");
                  break;
                case RIGHT :
                  printf("RIGHT\n");
                  break;
                default :
                  printf("error\n");
                 break;
            }
          }
        }
      }
    }
    k++;
    //k=12;
  }
  //printTree(initialNode);
  return initialNode;

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

int validCoordSym(Coord coord, FloatCoord max)
{
  return (coord.x >= 0 && coord.x <= max.x) && (coord.y >= 0 && coord.y <= max.y) && (coord.z >= 0 && coord.z <= max.z) ;
}
