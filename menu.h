#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "linmath.h"

#include "log.h"
#include "snake.h"
#include "fonts.h"

#define MAX_MENU_SIZE 20
#define DEFAULT_MIN_FONT_SIZE 5
#define DEFAULT_MAX_FONT_SIZE 20

typedef enum ItemType {ITEM ,ROW, COLLUM} ItemType;

typedef enum Action {NONE, OPEN, CLOSE, TEST} Action;

typedef struct Color {
  float r;
  float g;
  float b;
  float a;
} Color;

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

typedef struct Menu Menu;

typedef struct Item
{
  ItemType type;
  float bbox[4];
  float margin[4]; //left top right bottom
  Descriptor descriptor;
  Menu *menu;
} Item;

struct Menu{
  float margin[4];//left top right bottom
  float bbox[4];
  int size;
  Item **item;
  pthread_mutex_t *mutex;
};

//Global pointer
Menu *mymenu;
//Menu Prototype
int initMenu(Menu **menu);
//maybe init menu item
int getMenuMargin(Menu *menu, float margin[]);
int getMenuBbox(Menu *menu, float bbox[]);
int getMenuSize(Menu *menu, int *size);

int setMenuBbox(Menu *menu, float bbox[]);
int setMenuMargin(Menu *menu, float margin[]);
int setMenuSize(Menu *menu, int size);

//Item Prototype
int initItem(Item **item);

int getItemType(Item *item, ItemType *itemType);
int getItemDescriptor(Item *item, Descriptor *descriptor);
int getItemMenu(Item *item, Menu *menu);
int getItemBbox(Item *item, float bbox[]);
int getItemMargin(Item *item, float margin[]);

int setItemBbox(Item *item,float bbox[]);
int setItemMargin(Item *item,float margin[]);
int setItemType(Item *item, ItemType itemType);
int setItemDescriptor(Item *item, Descriptor descriptor);
int setItemMenu(Item *item, Menu *menu);

//Building Prototype;
int addItemToMenu(Menu *menu, Item *item);
int calcMenu(Menu *menu);
int reshapeMenu(Menu *menu,int width, int height); //See later
int reduceMenu(Menu *menu);
int increaseMenu(Menu *menu);

#endif
