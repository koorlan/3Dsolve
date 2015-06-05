/**
 * Projet application - 3DSolve
 * Version : 1.0
 *
 * Programme pour la résolution et la manipulation virtuelle de
 * casse-tête de type "Snake Cube"
 *
 * Auteurs :
 * 	- L.Aubry <lisa.aubry@insa-cvl.fr>
 * 	- A.Chazot <alban.chazot@insa-cvl.fr>
 * 	- K.Colas <korlan.colas@insa-cvl.fr>
 * 	- A.Gourd <anthony.gourd@insa-cvl.fr>
 *
 * Tuteur :
 * 	- P.Clemente <patrice.clemente@insa-cvl.fr>
 *
 * Date : 11 / 06 / 15
 *
 * INSA - Centre Val De Loire
 * 2014-2015
 * Promotion 2017
 *
 */

/**
 * @defgroup Menu Menu
 *
 * @brief Module de gestion des menus.
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "linmath.h"


#include "log.h"
#include "object.h"
#include "snake.h"
#include "fonts.h"


#define MAX_MENU_SIZE 15
#define DEFAULT_MIN_FONT_SIZE 5
#define DEFAULT_MAX_FONT_SIZE 20

/**
 * @ingroup Menu
 * @enum MenuType
 * @brief Définit le type de l'item.
 *
 * @var MenuType::ROW
 * Un menu en ligne
 * @var MenuType::COLLUM
 * Un menu en colonne
 */
typedef enum MenuType {ROW, COLUMN} MenuType;

/**
 * @ingroup Menu
 * @enum Action
 * @brief Définit une action pour un item.
 *
 * @var Action::NONE
 * Aucune action
 * @var Action::MENU
 * Pour un item de type menu
 * @var Action::RESET
 * [UNUSED] Action pour reset l'application
 * @var Action::TEST
 * [UNUSED] Utile pour tester des actions
 * @var Action::BACKAPP
 * [UNUSED] Action pour retourner au menu principal
 * @var Action::EXIT
 * Quitter l'application
 * @var Action::HELP
 * Affiche le menu d'aide
 * @var Action::ABOUT
 * Affiche l'ecran "A propos"
 * @var Action::OPTION
 * [UNUSED] ---
 * @var Action::LOADSNAKE
 * Charge un model de serpent a resoudre
 * @var Action::SOLUTION
 * [UNUSED] Regarde si des solutions ont déja été pré calculé
 * @var Action::GENSOL
 * [UNUSED] Pour amélioration ne pas résoudre le serpent au chargement, permet la génération de solutions
 * @var Action::LOADSOL
 * Charge une solution pré-calculé
 * @var Action::NEXTSOL
 * [UNUSED] Systeme de pagination de solutions
 * @var Action::PREVSOL
 * [UNUSED] Systeme de pagination de solutions
 * @var Action::EXPAND
 * [UNUSED] Pour avoir la vue eclatée du serpent (En jeu appuyer sur Entrée pour la vue éclatée)
 */
typedef enum Action {NONE, MENU,RESET, TEST,BACKAPP,EXIT,HELP,ABOUT,OPTION,LOADSNAKE,SOLUTION,GENSOL,LOADSOL,NEXTSOL,PREVSOL,EXPAND} Action;

/**
 * @ingroup Menu
 * @enum State
 * @brief Définit un état pour un menu.
 *
 * @var Action::OPEN
 * Menu ouvert et donc visible (Traitement par le thread de rendu)
 * @var Action::CLOSE
 * Menu fermé et donc invisible (Traitement par le thread de rendu)
 */
typedef enum State {OPEN, CLOSE} State;

/**
 * @ingroup Menu
 * @struct Color
 * @brief Une couleur définit en RGBA.
 *
 * Les composantes RGBA sont donnée dans l'intervalle [0 - 1]
 * @var Color::r
 * Composante rouge de la couleur désirée
 * @var Color::g
 * Composante verte de la couleur désirée
 * @var Color::b
 * Composante bleu de la couleur désirée
 * @var Color::a
 * Composante alpha (transparence) de la couleur désirée
 */
typedef struct Color {
  float r;
  float g;
  float b;
  float a;
} Color;

/**
 * @ingroup Menu
 * @struct Descriptor
 * @brief Descripteur d'un item.
 *
 * Le Descripteur contient les différantes valeurs caractérisant un item.
 *
 * @var Descripteur::name
 * Le texte à afficher
 * @var Descripteur::font
 * La police de texte à utiliser pour rendre le texte
 * @var Descripteur::fontSize
 * La taille de la police de texte
 * @var Descripteur::minFontSize
 * La taille minimale de la police de texte
 * @var Descripteur::maxFontSize
 * La taille maximale de la police de texte à utiliser
 * @var Descripteur::color
 * La couleur du texte
 * @var Descripteur::Action
 * L'action du descripteur de l'objet
 * @var Descripteur::bbox
 * Contient les différantes valeurs permettant de caractériser la zone de
 * rendu de l'item
 */
typedef struct Descriptor
{
  char *name;
  FTGLfont *font;
  int fontSize;
  int minFontSize;
  int maxFontSize;
  Color color;
  Action action;
  float bbox[6];
} Descriptor;


/**
 * @ingroup Menu
 * @struct Item
 * @brief Un élément d'un menu.
 *
 * Les items sont les plus petits éléments d'un menu, au clique, ils peuvent
 * soit déclancher une action ou bien faire apparaitre un autre menu
 * en fonction de Item::type.
 *
 * @var Item::bbox
 * Les caractéristique de la zone de rendu de l'item
 * @var Item::margin
 * Les marges à appliquer à l'item
 * @var Item::descriptor
 * Le descripteur de l'item
 * @var Item::menu
 * Le menu à dérouler au clique. Ce pointeur est égale à NULL si
 * @var Item::bboxRel
 * Tableau de 2 vecteurs X,Y calculés pour la boite de rendu relatives (-1.f à 1.f)
 * ![Illustration bboxRel](img/boxdim.png)
 * @var Item::marginRel
 * Tableau de 2 vecteurs X,Y calculés pour les marges relatives (-1.f à 1.f)
 * ![Illustration bboxRel](img/boxdim.png)
 */

typedef struct Menu Menu;

typedef struct Item
{
  float bbox[4];
  float margin[4]; //left top right bottom
  Descriptor descriptor;
  Menu *menu;
  float bboxRel[4];
  float marginRel[4];
} Item;

/**
 * @ingroup Menu
 * @struct Menu
 * @brief Définit un menu.
 *
 * Un menu est un ensemble d'item qui peuvent eux-même déclancher l'ouverture
 * de sous-menu autorisant ainsi d'avoir des menus en cascade.
 *
 * @var Menu::margin
 * Les marges à appliquer au menu, dans l'ordre : Gauche, Haut, Droite, Bas.
 * @var Menu::bbox
 * Valeurs de description de la zone de rendu du menu
 * @var Menu::bboxRel
 * Tableau de 2 vecteurs X,Y calculés pour la boite de rendu relatives (-1.f à 1.f)
 * ![Illustration bboxRel](img/boxdim.png)
 * @var Menu::size
 * Nombre d'item dans le menu
 * @var Menu::item
 * Tableau dynamique de pointeur sur items. Ce sont les items présents
 * dans le menu
 * @var Menu::mesh
 * points et uv-map de l'objet 3D
 * @var Menu::selected
 * ID relatif du numéro de l'item selectionné dans ce menu ( ne peux pas dépasser Menu::size )
 * @var Menu::opened
 * ID d'un item de type menu qui est ouvert ( Menu::state == OPEN )
 */
struct Menu{
  State state;
  MenuType type;
  float margin[4];
  float bbox[4];
  float bboxRel[4];
  int size;
  Item **item;
  Object *mesh;
  int selected;
  int opened;
};

/**
 * @ingroup Menu
 * @brief Initialise un menu.
 * @param  menu le menu à initialiser
 * @return      1 si l'initialisation c'est bien passée, 0 sinon
 */
int initMenu(Menu **menu);

/**
 * @ingroup Menu
 * @brief Permet de détruir récursivement un menu et ses sous-menu
 *
 * @param menu Le menu à détruir
 */
void menuDestroy(Menu* menu);

/**
 * @ingroup Menu
 * @brief Permet d'obtenir les marges d'un menu
 * @param  menu   Le menu
 * @param  margin le tableau dans lequel on veux récupérer les valeurs des marges
 * @return        Valeur de contrôle 1 si l'opération s'est bien passé, 0 sinon
 */
int getMenuMargin(Menu *menu, float margin[]);

/**
 * @ingroup Menu
 * @brief Permet d'obtenir le bbox d'un menu
 * @param  menu le menu
 * @param  bbox la bbox dans laquelle récupérer les valeurs
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getMenuBbox(Menu *menu, float bbox[]);

/**
 * @ingroup Menu
 * @brief Permet de récupérer le nombre d'item contenu dans un menu
 * @param  menu le menu
 * @param  size l'entier dans lequel récupérer la Valeur
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getMenuSize(Menu *menu, int *size);

/**
 * @ingroup Menu
 * @brief Modifie les valeurs de la bbox
 * @param  menu le menu
 * @param  bbox les nouvelles valeurs de le bbox
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int setMenuBbox(Menu *menu, float bbox[]);

/**
 * @ingroup Menu
 * @brief Modifie les valeurs des marges pour un menu
 * @param  menu le menu
 * @param  margin les nouvelles valeurs de marges
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int setMenuMargin(Menu *menu, float margin[]);

/**
 * @ingroup Menu
 * @brief Modifie le nombre d'item afficher par le menu.
 *
 * @param  menu le menu
 * @param  size le nouveau nombre d'item
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int setMenuSize(Menu *menu, int size);

/**
 * @ingroup Menu
 * @brief Initialise un Item.
 *
 * @param  item l'item à initialiser
 * @return      1 si l'initialisation est réussie, 0 sinon
 */
int initItem(Item **item);


/**
 * @ingroup Menu
 * @brief Permet de récupérer le descripteur d'un item
 * @param  item     l'item
 * @param  descriptor le pointeur dans lequel récupérer le descripteur
 * @return          valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getItemDescriptor(Item *item, Descriptor *descriptor);

/**
 * @ingroup Menu
 * @brief Permet de récupérer le menu d'un item
 * @param  item     l'item
 * @param  menu le pointeur dans lequel récupérer le menu
 * @return          valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getItemMenu(Item *item, Menu *menu);

/**
 * @ingroup Menu
 * @brief Permet de récupérer la bbox d'un item
 * @param  item     l'item
 * @param  bbox  la bbox dans laquelle récupérer les valeurs
 * @return          valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getItemBbox(Item *item, float bbox[]);

/**
 * @ingroup Menu
 * @brief Permet de récupérer les marges d'un item
 * @param  item     l'item
 * @param  mergin le tableau dans lequel récupérer les valeurs
 * @return          valeur de contrôle, 1 si l'opération s'est bien passé, 0 sinon
 */
int getItemMargin(Item *item, float margin[]);

/**
 * @ingroup Menu
 * @brief Permet de modifier la bbox d'un item.
 *
 * @param  item l'item
 * @param  bbox les nouvelles valeurs de la bbox
 * @return      valeur de contrôle, s si l'opération s'est bien passé, 0 sinon
 */
int setItemBbox(Item *item,float bbox[]);

/**
 * @ingroup Menu
 * @brief Permet de modifier les marges d'un item.
 *
 * @param  item l'item
 * @param  margin les nouvelles valeurs de marge
 * @return      valeur de contrôle, s si l'opération s'est bien passé, 0 sinon
 */
int setItemMargin(Item *item,float margin[]);


/**
 * @ingroup Menu
 * @brief Permet de modifier le descripteur d'un item
 *
 * @param  item l'item
 * @param  descriptor le nouveau descripteur
 * @return      valeur de contrôle, s si l'opération s'est bien passé, 0 sinon
 */
int setItemDescriptor(Item *item, Descriptor descriptor);

/**
 * @ingroup Menu
 * @brief Permet de modifier le menu d'un item.
 *
 * @param  item l'item
 * @param  menu le nouveau menu
 * @return      valeur de contrôle, s si l'opération s'est bien passé, 0 sinon
 */
int setItemMenu(Item *item, Menu *menu);

/**
 * @ingroup Menu
 * @brief Ajoute un item à un menu
 * @param  menu le menu auquel ajouter l'item
 * @param  item l'item à ajouter
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int addItemToMenu(Menu *menu, Item *item);

/**
 * @ingroup Menu
 * @brief Calcul la taille du menu.
 *
 * Remplie les bbox de tous les items présents dans le menu
 * @param  menu le menu
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int calcMenu(Menu *menu);

/**
 * @ingroup Menu
 * @brief Recalcul les tailles des items et du menu en prenant en compte la taille
 * de la fenêtre.
 *
 * @param  menu   le menu
 * @param  width  largeur de la fenêtre
 * @param  height hauteur de la fenêtre
 * @return        Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int reshapeMenu(Menu *menu,int width, int height);

/**
 * @ingroup Menu
 * @brief Réduit la taille d'un menu.
 *
 * @param  menu le menu
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int reduceMenu(Menu *menu);

/**
 * @ingroup Menu
 * @brief Augmente la taille d'un menu.
 *
 * @param  menu le menu
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int increaseMenu(Menu *menu);

/**
 * @ingroup Menu
 * @brief Calcul et remplissage des coordonées RELATIVES de chaque Item ainsi que la taille relative du Menu
 *
 * @param  menu le menu
 * @param  width Largeur de la fenêtre
 * @param  menu Hauteur de la fenêtre
 * @return      Valeur de contrôle, 1 si l'opération s'est bien passée, 0 sinon
 */
int calcMenuMesh(Menu *menu,int width, int height);


#endif //MENU_H
