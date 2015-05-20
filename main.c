/**
 * @mainpage
 *
 * Ce programme résoud, de manière calculatoire, des "Snake Cube". La méthode
 * de recherche de solution est itérative et utilise un arbre n-aire.
 *
 * Le principe est le suivant :
 * 	1. Un algorithme de recherche de symétrie élime les point de départ
 * 	symétique afin de réduire au maximum le temps de calcule.
 * 	2. Un autre algorithme essaie tous les chemins (combinaison de placement
 * 	des éléments du snake) qu'il est possible de généré depuis les points
 * 	trouvé à l'étape 1.
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
 * Projet tutoré par Patrice Clemente <patrice.clement@insa-cvl.fr>
 *
 * INSA Centre Val de Loire : Année 2014-2015
 *
 * Promotion 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "context.h"
#include "log.h"
#include "snake.h"
#include "resolver.h"

int main ( int argc, char ** argv )
{
	if(!logStart())
	{
		logError ("[MAIN.] Could not start log\n");
		return EXIT_FAILURE;
	}

	char* filePath = malloc(50);
	if(argc == 2)
		strncpy(filePath, argv[1], 50);
	else
		strncpy(filePath, "Snakes/snake.snake", 50);

	Snake* snake = snakeInit(filePath);
	free(filePath);

	if(snake == NULL)
		return -10;
		
	Context* context = contextCreate ();
	if ( !context )
	{
		logError ("[MAIN.] Could not create context\n");
		return EXIT_FAILURE;
	}
	context->snake = snake;

	resolverSolveSnake(snake);

	struct timespec time1;
	struct timespec time2;
	time1.tv_sec = 0;
	time1.tv_nsec = 1000000;

	contextInit ( context );

	while (context->running)
	{
		getInput(context);

		nanosleep(&time1, &time2);
	}

	contextDestroy ( context );

	snakeDestroy ( snake );

	logWrite ("[MAIN.] Terminated\n");
	return EXIT_SUCCESS;
}
