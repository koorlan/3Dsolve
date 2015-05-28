#ifndef APPLICATION_H
#define APPLICATION_H

#ifdef _WIN32
    #include <io.h>
    #include <time.h>
    #include <direct.h>
    #include <conio.h>
    #include <ctype.h>
    #include <windows.h>
#endif

#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "log.h"
#include "player.h"
#include "menu.h"

/**
 * @defgroup Application Application
 *
 * @brief Contrôle l'application.
 */

/**
 * @ingroup Application
 * @struct Application
 * @brief Représente l'état de l'application.
 *
 * Application est utilisé comme un singleton. Elle stocke les informations
 * sur l'état de l'application ainsi que diverse ressources comme la liste diverse
 * snake disponible etc...
 *
 * @var Application::snakeNames
 * Liste des snakes disponibles.
 * @var Application::snakeNumber
 * Nombre de snake disponible (taille de Application::snakeNames)
 * @var Application::loadedSnake
 * Index du snake actuellement chargé
 * @var Application::snake
 * Le snake actuellement utilisé
 * (indexé Application::loadedSnake dans Application::snakeNames)
 * @var Application::player
 * Le joueur
 *
 * @var Application::menu
 * Le menu de l'application
 * @var Application::menuDepth
 * La profondeur du menu actuellement ouvert
 * @var Application::pathExplored
 * Le nombre de chemin exploré pour trouver toutes les solutions
 * @var Application::calculTime
 * Le temps pris pour le calcul des solutions
 */


typedef struct Application
{
    char** snakeNames;
    int snakeNumber;
    int loadedSnake;
    Snake* snake;
    Player* player;
    Player* solver;
    Menu *menu;
    int menuDepth;
    int itemSelected;
    int running;
    unsigned int pathExplored;
    double calculTime;
} Application;


//GLOBAL POINTER OMG OMG
Application* app;


/**
 * @ingroup Application
 * @brief Crée une nouvelle application
 * @return L'application créé ou NULL en cas d'erreur
 */
Application* applicationCreate();

/**
 * @ingroup Application
 * @brief Supprime \a app de la mémoire
 * @param app L'application à supprimer
 */
void applicationDestroy(Application* app);

/**
 * @ingroup Application
 * @brief Parcours le dossier "Snakes" de l'application à la recherche des snakes disponibles.
 *
 * Cette fonction remplit les champs Application::snakeNames et Application::snakeNumber
 * @param app   l'application
 */
void applicationFindSnakes(Application* app);

#endif //APPLICATION_H
