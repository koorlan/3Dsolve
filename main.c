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
 * @authors Lisa Aubry <lisa.aubry@insa-cvl.fr>, Alban Chazot <alban.chazot@insa-cvl.fr>,
 * Korlan Colas <korlan.colas@insa-cvl.fr>, Anthony Gourd <anthony.gourd@insa-cvl.fr>
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

char* help = "SolidSnake\n\tUtilise par défault le snake \"Snakes/snake.snake\" et 1 thread de calcul.\n\n"\
"Options disponibles :\n --help\n\t Affiche l'aide\n --snake [path]\n\t Permet de choisir un autre snake"\
" que celui par défaut en indiquant son chemin\n --threadNb [nombre]\n\t Permet de choisir le nombre de thread de"\
" calcul à utiliser.\n --noGraphics\n\t Permet de ne pas démarrer le context de rendu 3D (utilisé à des fins de debug)"\
". Note, si l'option --noGraphics est utilisé, l'application ne pourra pas calculer le temps de résolution.\n";

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
				*threadNb = tmp;
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
	int maxThreadNb = 1;
	int noGraphics = 0;
	checkArguments(argc, argv, &maxThreadNb, filePath, &noGraphics);

	app = applicationCreate();
	applicationFindSnakes(app);

	/// [2] Load snake from commande line arguments

	app->snake = snakeInit(filePath);
	free(filePath);

	if(app->snake == NULL)
	{
		logError("[MAIN.] Snake load failure");
		return EXIT_FAILURE;
	}
	/// [2]

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

		//Menu test;
		int i ;
		mymenu = NULL;
		initMenu(&mymenu);
		setMenuMargin(mymenu,(float []) {0.02f*context->screen_width, 0.02f*context->screen_height, 0.02f*context->screen_width, 0.02f*context->screen_height} );

		Item *tmpitem;
		for ( i = 0;  i <2; i++) {
			initItem(&(tmpitem));
			setItemType(tmpitem, ITEM);
			//setItemStartCoord(tmpitem,(float[2]){-1.f+0.2f*i,1.f-0.2f*i});
			setItemMargin(tmpitem,(float[]){5.f,10.f,5.f,10.f});
			setItemDescriptor(tmpitem,(struct Descriptor)
				{	.name="Default Text",
					.font=LoadFont("fonts/Libertine.ttf"),
					.fontSize = 20,
					.minFontSize = DEFAULT_MIN_FONT_SIZE,
					.maxFontSize = DEFAULT_MAX_FONT_SIZE,
					.color=(struct Color){.r=0.0f,.g=0.0f,.b=0.0f,.a=1.0f},
					.action= OPEN});
			ftglSetFontFaceSize(tmpitem->descriptor.font,tmpitem->descriptor.fontSize,72);
			addItemToMenu(mymenu,tmpitem);
		}

		//setItemDescriptor(mymenu->item[3],(struct Descriptor)
		//	{	.name="Custom Text",
		//		.font=LoadFont("fonts/Fipps-Regular.otf"),
		//		.fontSize = 25,
		//		.color=(struct Color){.r=0.0f,.g=0.0f,.b=1.0f,.a=1.0f},
		//		.action= OPEN});
		//ftglSetFontFaceSize(mymenu->item[3]->descriptor.font,mymenu->item[3]->descriptor.fontSize,72);
		//test relative fontSize
		calcMenu(mymenu);
		//End test menu
		/// [3]
	}

	/// [4] Application running

	#ifndef _WIN32
	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;
	#endif


	context->playmode = PM_PLAY;
	gplayer = playerInit ( app->snake );
	gplayer->selected = 0;
	gsolver = playerInit ( app->snake );
	gsolver->currentSolution = app->snake->solutions->head;

	if(!noGraphics)
		contextInit ( context );
	resolverSolveSnake(app->snake, NULL, maxThreadNb);

	if(!noGraphics)
	{
		//Boucle principale
		context->loading = 0;
		while (app->running)
		{
			getInput(context);
			#ifdef _WIN32
				Sleep(10);
			#else
				nanosleep(&time1, &time2);
			#endif
		}
	}
	/// [4]

	/// [5] Cleanning

	playerDestroy( gplayer );
	playerDestroy( gsolver );
	if(!noGraphics)
		contextDestroy ( context );
	applicationDestroy(app);
	/// [5]

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
