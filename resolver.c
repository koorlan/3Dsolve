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

pthread_mutex_t buildMutex = PTHREAD_MUTEX_INITIALIZER;

void resolverSolveSnake(Snake *snake)
{
    // Let's resolve <snake>
    logWrite("[RESOL] Starting resolution (snake size : %d x %d x %d)\n",snake->volume.max.x, snake->volume.max.y, snake->volume.max.z);

    double startTime = glfwGetTime();
    //long unsigned int exploredWayNb = 0;

    int initialVectorNb = 0;
    Tree rootNode = findInitialVectors(snake, &initialVectorNb);
    //Tree rootNode = createAllInitialVectors(snake->volume);

    logWrite("[RESOL] %d initial vector found\n", initialVectorNb);

    printSnake(*snake);

    if(initialVectorNb <= 0)
    {
        logError("[RESOL] No initial vector !\n");
        return;
    }

    Tree currentNode = rootNode->currentChild;

    int i = 0;

    ThreadArgs* args[initialVectorNb];

    pthread_t* threads = malloc(initialVectorNb * sizeof(pthread_t));
    Snake* snakes[initialVectorNb];

    for(i = 0; i < initialVectorNb; i++)
    {
        snakes[i] = malloc(sizeof(Snake));
        snakeCopy(snakes[i], snake);
        logWrite("[RESOL] Snake n°%d copied\n", i);
    }


    i = 0;
    while(currentNode != NULL)
    {
        args[i] = malloc(sizeof(ThreadArgs));
        logWrite("[RESOL] Starting thread %d\n", i);
        args[i]->snake = snakes[i];
        args[i]->rootNode = currentNode;
        currentNode = currentNode->brother;
        args[i]->rootNode->brother = NULL;
        args[i]->rootNode->parent = NULL;
        pthread_create(&(threads[i]), NULL, resolverSolveNode, args[i]);
        i++;
    }

    if(i == initialVectorNb)
        logWrite("[RESOL] All threads created !");

    Snake* currentSnake;
    for(i = 0; i < initialVectorNb; i++)
    {
        pthread_join(threads[i], NULL);

        currentSnake = snakes[i];
        if(currentSnake->solutions->size != 0)
        {
            logWrite("[RESOL] %d solutions for %d\n", currentSnake->solutions->size, i);
            Solution* currentSolution = currentSnake->solutions->head;
            while(currentSolution != NULL)
            {
                listSolutionInsert(snake->solutions, currentSolution->step);
                currentSolution = currentSolution->next;
            }
        }
        snakeDestroy(currentSnake);
        free(args[i]);
        logWrite("[RESOL] thread %d has terminated\n", i);
    }

    logWrite("[RESOL] Snake resolved\n");
    clock_t endTime = clock();
    if(endTime == (clock_t)(-1))
    logError("[RESOL] CPU clock time not available\n");

    double elapsedTime = glfwGetTime() - startTime;

    printf("\033[38;01mSnake resolved with\033[00m\033[31;01m %d \033[00m\033[38;01msolution(s)\n",
    snake->solutions->size);
    //printf("\033[31;01m%ld \033[00m\033[38;01mways have been explored\033[00m \n", exploredWayNb);
    logWrite ("[RESOL] Resolver Ended, found %d solutions in %lf seconds\n",snake->solutions->size,
    elapsedTime);
}

void* resolverSolveNode(void* args)
{
    ThreadArgs* tmp = args;
    Snake* snake = tmp->snake;
    int buildResult = -1;
    int exploredWayNb;

    Tree currentNode = tmp->rootNode;

    while(currentNode != tmp->rootNode || tmp->rootNode->hasPlayed == 0)
    {
        if(currentNode->hasPlayed == 0)
        {
            // Play the currentNode
            buildResult = buildChildren(currentNode, snake);
            if(buildResult == 1) // Build is OK
                currentNode = currentNode->currentChild;
            else if(buildResult == 2) // Snake edge -> solution found
            {
                snakeAddStep(snake, &(currentNode->currentChild->step));
                snakeAddSolution(snake);
                free(currentNode->currentChild);
                currentNode->currentChild = NULL;
                logWrite ("[RESOL] Resolver found a solution \n");
                snakeRewind(snake);

                exploredWayNb++;
            }
        }
        else
        {
            snake->volume.state [currentNode->step.coord.x]
                                [currentNode->step.coord.y]
                                [currentNode->step.coord.z] = FREE;

            if(currentNode->brother != NULL)
            {
                currentNode = currentNode->brother;
                free(currentNode->parent->currentChild);
                currentNode->parent->currentChild = currentNode;
            }
            else
            {
                exploredWayNb++;

                currentNode = currentNode->parent;
                free(currentNode->currentChild);
                currentNode->currentChild = NULL;
                snakeRewind(snake);
            }
        }
    }

    pthread_exit(NULL);

    /*logWrite("[RESOL] Snake resolved\n");
    clock_t endTime = clock();
    if(endTime == (clock_t)(-1))
    logError("[RESOL] CPU clock time not available");

    double elapsedTime = ((long double)(endTime - startTime)) / CLOCKS_PER_SEC;

    printf("\033[38;01mSnake resolved with\033[00m\033[31;01m %d \033[00m\033[38;01msolution(s) in\033[00m\033[31;01m %lf \033[38;01mseconds\033[00m\n", snake->solutions->size,
    elapsedTime);
    printf("\033[31;01m%ld \033[00m\033[38;01mways have been explored\033[00m \n", exploredWayNb);
    logWrite ("[RESOL] Resolver Ended, found %d solutions in %lf seconds\n",snake->solutions->size,
    elapsedTime);*/
}

Tree initTree()
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

    return elt;
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


void addInitialVector(Tree rootNode, int x, int y, int z, Dir newDir)
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
  newNode->brother = rootNode->currentChild ;
  newNode->currentChild = NULL;
  newNode->parent = rootNode;
  newNode->hasPlayed = 0;
  rootNode->currentChild = newNode;
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

int buildChildren(Tree currentNode, Snake * snake)
{
  int i = 0;

  Unit nextUnit;
  Tree newChild;

  Coord nCoord = calcCoord(currentNode->step.coord, currentNode->step.dir);

  if(validCoord(nCoord, snake->volume.max) && snake->volume.state[nCoord.x][nCoord.y][nCoord.z] == FREE)
  {
      snakeAddStep(snake, &(currentNode->step));
      snake->volume.state[currentNode->step.coord.x][currentNode->step.coord.y][currentNode->step.coord.z] = FILL;
      nextUnit = snakeGetNextUnit(snake);
      switch (nextUnit)
      {
        case EDGE:
            currentNode->hasPlayed = 1;
            currentNode->currentChild = NULL;
            newChild = malloc(sizeof(NodeTree));
            newChild->parent = currentNode;
            newChild->brother = NULL;
            newChild->hasPlayed = 1;
            newChild->currentChild = NULL;
            newChild->step.dir = currentNode->step.dir;
            newChild->step.coord.x = nCoord.x;
            newChild->step.coord.y = nCoord.y;
            newChild->step.coord.z = nCoord.z;
            currentNode->currentChild = newChild;
            return 2;
        break;
        case STRAIGHT:
          newChild = malloc(sizeof(NodeTree));
          newChild->parent = currentNode;
          newChild->brother = currentNode->currentChild;
          newChild->hasPlayed = 0;
          newChild->currentChild = NULL;
          newChild->step.dir = currentNode->step.dir;
          newChild->step.coord.x = nCoord.x;
          newChild->step.coord.y = nCoord.y;
          newChild->step.coord.z = nCoord.z;
          currentNode->currentChild = newChild;
          break;
        case CORNER:
          for ( i = 0; i < 6; i++)
          {
            if(cornerTruthTable[currentNode->step.dir][i])
            {
                newChild = malloc(sizeof(NodeTree));

                newChild->parent = currentNode;
                newChild->brother = currentNode->currentChild;
                newChild->hasPlayed = 0;
                newChild->currentChild = NULL;

                newChild->step.dir = i;
                newChild->step.coord.x = nCoord.x;
                newChild->step.coord.y = nCoord.y;
                newChild->step.coord.z = nCoord.z;

                currentNode->currentChild = newChild;
              }
          }
          break;
      default:
        break;
      }
      currentNode->hasPlayed = 1;
    return 1;
  }
  else
  {
      currentNode->hasPlayed = 1;
      return -1;
  }
}


void printTree (Tree rootNode)
{
  Tree tmpNode = rootNode->currentChild;
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
    line->a = (line->pointB.y - line->pointA.y) / (line->pointB.x - line->pointA.x);
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
          if (verticalAxis.a == -1)
          {
            typeOfAxis = "None";
            break;
          }
          else
          {
            cpyLine(&symmetryAxis, verticalAxis);
            typeOfAxis = "vertical";
          }
        break;
      case 1 :
          if (horizontalAxis.a == -1)
          {
            typeOfAxis = "None";
            break;
          }
          else
          { cpyLine(&symmetryAxis, horizontalAxis);
            typeOfAxis = "horizontal";
          }
          break;
      case 2 :
        if (diagonalAxis.a == -1)
          {
            typeOfAxis = "None";
            break;
          }
          else
          {  cpyLine(&symmetryAxis, diagonalAxis);
            typeOfAxis = "diagonal";
          }
        break;
      case 3 :
          if (slashAxis.a == -1)
          {
            typeOfAxis = "None";
            break;
          }
          else
          {
            cpyLine(&symmetryAxis, slashAxis);
            typeOfAxis = "slash";
          }
        break;

    }

    if(strcmp(typeOfAxis, "None")!=0)
    {
      float n = symmetryAxis.a*symmetryAxis.a + symmetryAxis.b*symmetryAxis.b;

      int cm = symmetryAxis.a*initialStep.coord.x + symmetryAxis.b*initialStep.coord.y;
      int xm = initialStep.coord.x + 2*symmetryAxis.a*(symmetryAxis.c - cm)/n;
      int ym = initialStep.coord.y + 2*symmetryAxis.b*(symmetryAxis.c - cm)/n;

      if(nCoord.x == xm && nCoord.y == ym && oppositeDir(initialStep.dir, nDir, typeOfAxis))
      {
        return 1;
      }
    }
  }
  return 0;
}

int lineCmp(Step step, Coord coord, Dir dir)
{
  return(step.coord.x == coord.x && step.coord.y == coord.y && step.coord.z == coord.z && dir == step.dir);
}

Tree findInitialVectors(Snake *snake, int * initialVectorNb)
{

  FloatCoord projectionCenter;

  Line verticalAxis;
  Line horizontalAxis;
  Line diagonalAxis;
  Line slashAxis;

  verticalAxis.a = -1;
  horizontalAxis.a = -1;
  diagonalAxis.a = -1;
  slashAxis.a = -1;

  Coord nCoord;

  int i, j, k, dirIndex, flag, dx, dy, depthValue, cptCube, decX, decY;
  cptCube = 0;
  depthValue = 0;
  dx = 1;
  dy = 1;
  k = 0;
  flag = 0;
  decX = -1;
  decY = -1;

  FloatCoord cubeCenter;
  cubeCenter.x = (float)(snake->volume.max.x-1)/2;
  cubeCenter.y = (float)(snake->volume.max.y-1)/2;
  cubeCenter.z = (float)(snake->volume.max.z-1)/2;

  printf("cube center %lf %lf %lf\n", cubeCenter.x, cubeCenter.y, cubeCenter.z);

  Tree initialNode = initTree();
  addInitialVector(initialNode, 0, 0, 2, UP);
  Tree tmpNode = initialNode->currentChild;
  (*initialVectorNb)++;

  if(snake->symetries[0] == 1)
  {
    dx = 2;
    decX = 0;

  }
  if(snake->symetries[1] == 1)
  {
    dy = 2;
    decY = 0;
  }
  if(snake->symetries[2] == 1)
  {
    depthValue = snake->volume.max.z / 2;
  }
  //select a face of the cube, parallel to the plan (0,x,y)
  while(snake->volume.max.z -1 - k >= depthValue)
  {
    cptCube = 0;
    //projected othogonal of the cube center on the considered face
    projectionCenter.x = cubeCenter.x;
    projectionCenter.y = cubeCenter.y;
    projectionCenter.z = snake->volume.max.z -1 - k;

    if(snake->symetries[0] == 1)
    {
      //initialize axe of symmetry
      verticalAxis.pointA.x = projectionCenter.x;
      verticalAxis.pointA.y = projectionCenter.y-1;
      verticalAxis.pointA.z = projectionCenter.z;
      verticalAxis.pointB.x = projectionCenter.x;
      verticalAxis.pointB.y = projectionCenter.y+1;
      verticalAxis.pointB.z = projectionCenter.z;

      //calculation of the linear equations
      linearEquation(&verticalAxis);

      cptCube ++;

      printf ("\n\033[38;01mVertical axis\033[00m\n");
    }

    if(snake->symetries[1] == 1)
    {
      horizontalAxis.pointA.x = projectionCenter.x-1;
      horizontalAxis.pointA.y = projectionCenter.y;
      horizontalAxis.pointA.z = projectionCenter.z;
      horizontalAxis.pointB.x = projectionCenter.x+1;
      horizontalAxis.pointB.y = projectionCenter.y;
      horizontalAxis.pointB.z = projectionCenter.z;

      linearEquation(&horizontalAxis);

      cptCube ++;

      printf ("\n\033[38;01mHorizontal axis\033[00m\n");
    }

    if(snake->symetries[2] == 1)
    {
      diagonalAxis.pointA.x = projectionCenter.x-1;
      diagonalAxis.pointA.y = projectionCenter.y+1;
      diagonalAxis.pointA.z = projectionCenter.z;
      diagonalAxis.pointB.x = projectionCenter.x+1;
      diagonalAxis.pointB.y = projectionCenter.y-1;
      diagonalAxis.pointB.z = projectionCenter.z;

      linearEquation(&diagonalAxis);

      cptCube ++;

      printf ("\n\033[38;01mDiagonal axis\033[00m\n");
    }

    if(snake->symetries[3] == 1)
    {
      slashAxis.pointA.x = projectionCenter.x+1;
      slashAxis.pointA.y = projectionCenter.y+1;
      slashAxis.pointA.z = projectionCenter.z;
      slashAxis.pointB.x = projectionCenter.x-1;
      slashAxis.pointB.y = projectionCenter.y-1;
      slashAxis.pointB.z = projectionCenter.z;

      linearEquation(&slashAxis);

      cptCube ++;

      printf ("\n\033[38;01mSlash axis\033[00m\n");

    }


    for(i=0; i <= snake->volume.max.x/dx + decX; i++)
    {
      for(j=0; j <= snake->volume.max.y/dy + decY; j++)
      {
        tmpNode=initialNode->currentChild;
        nCoord.x = i;
        nCoord.y = j;
        nCoord.z = projectionCenter.z;
        if(snake->volume.state[nCoord.x][nCoord.y][nCoord.z] == FREE)
        {
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
          if(((cptCube!=4) || (cptCube == 4 && validVectCube(nCoord, tempDir, snake->volume.max.z -1))) && validCoordSym(calcCoord(nCoord, tempDir), projectionCenter))
          {
            while(tmpNode!=NULL && !flag )
            {
              if(tmpNode->step.coord.z != nCoord.z)
              {
                flag = 0;
              }
              else if(!lineCmp(tmpNode->step, nCoord, tempDir))
                flag = symmetries(tmpNode->step, nCoord, tempDir, verticalAxis, horizontalAxis, diagonalAxis, slashAxis);
              else
                flag = 1;
              tmpNode=tmpNode->brother;
            }
            if(flag)
            {
              logWrite("[RESOL] Symmetry found\n");
              flag = 0;
            }
            else
            {
              logWrite("[RESOL] Symmetry not found\n");
              addInitialVector(initialNode, i, j, projectionCenter.z, tempDir);
              (*initialVectorNb)++;
            }
          }
        }
      }
      }
    }
    k++;
  }
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
    return  (coord.x >= 0 && coord.x < max.x) &&
            (coord.y >= 0 && coord.y < max.y) &&
            (coord.z >= 0 && coord.z < max.z);
}

int validCoordSym(Coord coord, FloatCoord max)
{
      return  (coord.x >= 0 && coord.x <= max.x) &&
              (coord.y >= 0 && coord.y <= max.y) &&
              (coord.z >= 0 && coord.z <= max.z);
}

Tree createAllInitialVectors(Volume volume)
{
    Tree root = initTree();
    int x, y, z, dir;
    for(x=0; x < volume.max.x; x++)
        for(y=0; y < volume.max.y; y++)
            for(z=0; z < volume.max.z; z++)
                for(dir=0; dir < 6; dir++)
                    addInitialVector(root, x, y, z, dir);

    return root;
}

int validVectCube(Coord nCoord, Dir dir, int max)
{
  if(nCoord.z == 0 || nCoord.z == max || (nCoord.x != 0 && nCoord.x != max && nCoord.y != 0 && nCoord.y != 0))
    return 1;

  Coord vectCoord;
  vectCoord = calcCoord(nCoord, dir);
  if(vectCoord.x == 0 || vectCoord.y == 0 )
    return 0;
  else
    return 1;
}
