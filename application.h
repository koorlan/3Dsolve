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


#define AS_HOME		0
#define AS_GAME		1
#define AS_HELP		2
#define AS_ABOUT	3
#define AS_LOAD		4

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
 * @var Application::solver
 * Un "joueur" utiliser afin de simuler un joueur qui connaitrait la solution. Il
 * est utilisé pour permettre à l'utilisateur de visualiser les solutions.
 * @var Application::menu
 * Le menu de l'application
 * @var Application::menuDepth
 * La profondeur du menu actuellement ouvert
 * @var Application::itemSelected
 * L'Identifiant de l'item (système des Menu) sur lequel l'utilisateur a cliqué
 * @var Application::running
 * Ce champ est utilisé comme un flag afin d'indiquer si l'application est active ou non.
 * C'est la mise à 0 de ce champ qui permet de quitter l'application.
 * @var Application::state
 * Décrit l'état de l'application : en jeu, en calcul de solution, en attente à l'écran
 * d'accueil, affichage l'aide ou du menu "à propos". Cette variable conditionne le comportement
 * du module graphique.
 * @var Application::maxThread
 * Le nombre maximal de thread de l'application est autorisée à utiliser pour résoudre le snake.
 * Par défaut, cette valeur est mise à -1 lors de l'initialisation de l'application (applicationCreate).
 * La valeur -1 signifie que l'application est autorisée à utiliser autant de thread de calcul qu'il
 * y a de points de départ pour le snake sélectionné.
 * @var Application::updateSolutionMenu
 * Ce champ est utilisé comme un flag pour informer le thread principal de la nécessité ou non
 * d'actualiser le menu des solutions. Utilisé par le thread de calcul une fois toutes les
 * solutions trouvées.
 * @var Application::buttonPushed
 * Identifiant du boutton (gauche ou droite) qui a été cliqué.
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
    int state;
    int maxThread;
    int updateSolutionMenu;
    int buttonPushed;
} Application;

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

/**
 * @ingroup Application
 * @brief Initialise le menu de l'application
 *
 * @param  app    L'application
 * @param  width  La largeur de la fenêtre
 * @param  height La hauteur de la fenêtre
 * @return        Valeur de contrôle : 1 si OK 0 sinon
 */
int applicationInitMainMenu(Application *app,int width, int height);

#endif //APPLICATION_H
