/*
  Projet application - 3DSolve
  Version : 1.0

  Programme pour la résolution et la manipulation virtuelle de
  casse-tête de type "Snake Cube"

  Auteurs :
  	- L.Aubry <lisa.aubry@insa-cvl.fr>
  	- A.Chazot <alban.chazot@insa-cvl.fr>
  	- K.Colas <korlan.colas@insa-cvl.fr>
  	- A.Gourd <anthony.gourd@insa-cvl.fr>

  Tuteur :
  	- P.Clemente <patrice.clemente@insa-cvl.fr>

  Date : 11 / 06 / 15

  INSA - Centre Val De Loire
  2014-2015
  Promotion 2017

 */

/**
 * @mainpage
 *
 * Ce programme résoud, de manière calculatoire, des "Snake Cube". La méthode
 * de recherche de solution est itérative et utilise un arbre n-aire.
 *
 * Le principe est le suivant :
 * 	1. Un algorithme de recherche de symétrie élime les points de départ
 * 	symétiques afin de réduire au maximum le temps de calcul.
 * 	2. Un autre algorithme essaie tous les chemins (combinaison de placement
 * 	des éléments du snake) qu'il est possible de générer depuis les points
 * 	trouvés à l'étape 1.
 * 	3. Les chemins qui aboutissent (qui permettent de placer tous les éléments
 * 	du snake de manière valide) sont des solutions.
 * 	4. Les solutions sont ensuite présentées à l'utilisateur dans une scène de
 * 	rendu en 3 dimentions. L'utilisateur peut regarder pas-à-pas la résolution
 * 	du casse-tête.
 *
 * @authors
 * * Lisa Aubry <lisa.aubry@insa-cvl.fr>
 * * Alban Chazot <alban.chazot@insa-cvl.fr>
 * * Korlan Colas <korlan.colas@insa-cvl.fr>
 * * Anthony Gourd <anthony.gourd@insa-cvl.fr>
 * @date Juin 2015
 *
 * Projet tutoré par Patrice Clemente <patrice.clemente@insa-cvl.fr>
 *
 * INSA Centre Val de Loire : Année 2014-2015
 *
 * Promotion 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
	#include <windows.h>
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // Other kinds of Mac OS
		#endif
#elif __linux
    #include <unistd.h>
#else
		#error "Unspported Plateform"
#endif

#include <time.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"
#include "player.h"
#include "fonts.h"
#include "menu.h"
#include "application.h"

char* help = "SolidSnake\n\tUtilise par défault le snake \"Snakes/snake.snake\"\n\n"\
"Options disponibles :\n --help\n\t Affiche l'aide\n --snake [path]\n\t Permet de choisir un autre snake"\
" que celui par défaut en indiquant son chemin\n --threadNb [nombre]\n\t Permet de choisir le nombre de thread de"\
" calcul à utiliser.\n";

void checkArguments(int argc, char** argv, int* threadNb, char* snakeFile, int* noGraphics)
{
	int currentArgIndex;
	char* currentArg;
	for(currentArgIndex = 1; currentArgIndex < argc; currentArgIndex++)
	{
		currentArg = argv[currentArgIndex];
		if(strncmp(currentArg, "--help", 6) == 0)
		{
			printf("%s", help);
			exit(EXIT_SUCCESS);
		}
		else if(strncmp(currentArg, "--snake", 7) == 0)
		{
			currentArgIndex++;
			if(currentArgIndex < argc &&
			strstr(argv[currentArgIndex], ".snake") != NULL)
				strncpy(snakeFile, argv[currentArgIndex], 50);
			else
			{
				printf("%s", help);
				exit(EXIT_FAILURE);
			}
		}
		else if(strncmp(currentArg, "--threadNb", 10) == 0)
		{
			int tmp;
			currentArgIndex++;
			if(currentArgIndex < argc &&
			sscanf(argv[currentArgIndex], "%d", &tmp) == 1)
			{
				*threadNb = tmp;
				if(tmp <= 0)
				{
					printf("%s", help);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				printf("%s", help);
				exit(EXIT_FAILURE);
			}
		}
		else if(strncmp(currentArg, "--noGraphics", 12) == 0)
			*noGraphics = 1;
		else
		{
			printf("%s", help);
			exit(EXIT_FAILURE);
		}
	}
}

int main ( int argc, char ** argv )
{
	/// [1] Init log system
	if(!logStart())
	{
		logError ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}
	/// [1]


	char* filePath = malloc(50);
	strncpy(filePath, "Snakes/snake.snake", 19);
	int noGraphics = 0;
	app = applicationCreate();
	app->maxThread = -1;

	checkArguments(argc, argv, &(app->maxThread), filePath, &noGraphics);
	applicationFindSnakes(app);

	/// [2] Load snake from commande line arguments

	app->snake = snakeInit(filePath);
	free(filePath);

	if(app->snake == NULL)
	{
		logError("[MAIN.] Snake load failure\n");
		return EXIT_FAILURE;
	}
	/// [2]

	pthread_attr_t attr;
	pthread_t solverThread;
	ThreadArgs* args = malloc(sizeof(ThreadArgs));
	args->snake = app->snake;
	args->rootNode = NULL;
	args->resetSolutionMenu = 1;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&solverThread, &attr, resolverSolveSnake, args);

	/// [3] Context and menu initialization
	Context* context;
	if(!noGraphics)
	{
		context = contextCreate ();
		if ( !context )
		{
			logError ("[MAIN.] Could not create context\n");
			return EXIT_FAILURE;
		}
		applicationInitMainMenu(app,context->screen_width, context->screen_height);
	}
	/// [3]

	/// [4] Application running

	#ifndef _WIN32
	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;
	#endif

	if(!noGraphics)
	{
		context->playmode = PM_PLAY;
		gplayer = playerInit ( app->snake );
		gplayer->selected = -1;
		gsolver = playerInit ( app->snake );
		gsolver->currentSolution = app->snake->solutions->head;
		contextInit ( context );
	}

	if(!noGraphics)
	{
		//Boucle principale
		while (app->running)
		{
			getInput(context);
			#ifdef _WIN32
				Sleep(10);
			#else
				nanosleep(&time1, &time2);
			#endif

			if(app->updateSolutionMenu == 1)
			{
				playerDestroy(gplayer);
				gplayer = playerInit ( app->snake );
				gplayer->selected = -1;
				playerDestroy(gsolver);
				gsolver = playerInit(app->snake);
				gsolver->currentSolution = app->snake->solutions->head;

				int i;
				//Snake Solution
				app->menu->item[2]->menu->size = 0;
				for (i=0 ; i<app->snake->solutions->size && i<MAX_MENU_SIZE; i++)
				{
					char buf[255];
					char snakeSolution[255] = "solution n°\0";
					sprintf(buf,"%d",i+1);
					strcat(snakeSolution,buf);
					strcpy(app->menu->item[2]->menu->item[i]->descriptor.name,snakeSolution);
					app->menu->item[2]->menu->size ++;
				}
				calcMenu(app->menu->item[2]->menu);
				calcMenuMesh(app->menu->item[2]->menu,context->screen_width,context->screen_height);
				app->updateSolutionMenu = 0;
			}
		}
	}
	/// [4]

	/// [5] Cleanning

	playerDestroy( gplayer );
	playerDestroy( gsolver );
	if(!noGraphics)
		contextDestroy ( context );

	menuDestroy(app->menu);
	applicationDestroy(app);
	/// [5]

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
