#include "menu.h"

//Menu Prototype
int initMenu(Menu **menu){

  *menu = malloc(sizeof(Menu));
  if(*menu == NULL)
    return 0;
  memset((*menu)->margin, 0.f,4*sizeof(float));
  memset((*menu)->bbox, 0.f,4*sizeof(float));
  (*menu)->size = 0;
  (*menu)->item = malloc( MAX_MENU_SIZE * sizeof(Item)); // A voir si pas sizeof(*(*menu)->item)
  return 1;
}

int getMenuMargin(Menu *menu, float margin[]){
  if (menu == NULL || margin == NULL)
    return 0;
  margin[0] = menu->margin[0];
  margin[1] = menu->margin[1];
  margin[2] = menu->margin[2];
  margin[3] = menu->margin[3];
  return 1;
}

int getMenuBbox(Menu *menu, float bbox[]){
  if (menu == NULL || bbox == NULL)
    return 0;
  bbox[0] = menu->bbox[0];
  bbox[1] = menu->bbox[1];
  bbox[2] = menu->bbox[2];
  bbox[3] = menu->bbox[3];
  return 1;
}

int getMenuSize(Menu *menu, int *size){
  if (menu == NULL || size == NULL)
    return 0;
  *size = menu->size;
  return 1;
}

int setMenuMargin(Menu *menu, float margin[]){
  if (menu == NULL || margin == NULL)
    return 0;
  //if (coord[0] < -1.f || coord[0] > 1.f || coord[1] < -1.f || coord[1] > 1.f)
  //  return -1;
  menu->margin[0] = margin[0];
  menu->margin[1] = margin[1];
  menu->margin[2] = margin[2];
  menu->margin[3] = margin[3];
  return 1;
}

int setMenuBbox(Menu *menu, float bbox[]){
  if (menu == NULL || bbox == NULL)
    return 0;
//  if (coord[0] < -1.f || coord[0] > 1.f || coord[1] < -1.f || coord[1] > 1.f)
  //  return -1;
  menu->bbox[0] = bbox[0];
  menu->bbox[1] = bbox[1];
  menu->bbox[2] = bbox[2];
  menu->bbox[3] = bbox[3];
  return 1;
}

int setMenuSize(Menu *menu, int size){
  if (menu == NULL || size < 0 || size > MAX_MENU_SIZE)
    return 0;
  menu->size = size;
  return 1;
}


//Item Prototype
int initItem(Item **item){
  *item = malloc(sizeof(Item));
  if( *item == NULL)
    return 0;
  (*item)->type = ITEM;

  memset((*item)->bbox, 0.f,4*sizeof(float));
  memset((*item)->margin, 0.f,4*sizeof(float));

  (*item)->descriptor.name = NULL;
  (*item)->descriptor.font = NULL;
  (*item)->descriptor.fontSize = 0;
  (*item)->descriptor.color.r = 0.f;
  (*item)->descriptor.color.g = 0.f;
  (*item)->descriptor.color.b = 0.f;
  (*item)->descriptor.color.a = 0.f;
  (*item)->descriptor.action = NONE;
  memset((*item)->descriptor.bbox, 0.f,6*sizeof(float));

  (*item)->menu = NULL;
  return 1;
}

int getItemType(Item *item, ItemType *itemType){
  if( item == NULL || itemType == NULL)
    return 0;
  *itemType = item->type;
  return 1;
}

int getItemBbox(Item *item, float bbox[]){
  if( item == NULL || bbox == NULL)
    return 0;
  bbox[0] = item->bbox[0];
  bbox[1] = item->bbox[1];
  bbox[2] = item->bbox[2];
  bbox[3] = item->bbox[3];
  return 1;
}

int getItemMargin(Item *item, float margin[]){
  if( item == NULL || margin == NULL)
    return 0;
  margin[0] = item->margin[0];
  margin[1] = item->margin[1];
  margin[2] = item->margin[2];
  margin[3] = item->margin[3];
  return 1;
}



int getItemDescriptor(Item *item, Descriptor *descriptor){
  if( item == NULL || descriptor == NULL)
    return 0;
  descriptor->name = item->descriptor.name ;
  descriptor->font = item->descriptor.font ;
  descriptor->fontSize = item->descriptor.fontSize ;
  descriptor->color.r = item->descriptor.color.r ;
  descriptor->color.g = item->descriptor.color.g ;
  descriptor->color.b = item->descriptor.color.b ;
  descriptor->color.a = item->descriptor.color.a ;
  descriptor->action = item->descriptor.action ;
  descriptor->bbox[0] = item->descriptor.bbox[0];
  descriptor->bbox[1] = item->descriptor.bbox[1];
  descriptor->bbox[2] = item->descriptor.bbox[2];
  descriptor->bbox[3] = item->descriptor.bbox[3];

  return 1;
}
int getItemMenu(Item *item, Menu *menu){
  if( item == NULL || menu == NULL)
    return 0;
  if ( item->type != ROW || item->type != COLLUM )
    return -1; // Only ROW and COLLUM can have a pointer to a menu.
  menu = item->menu;
  return 1;
}

int setItemType(Item *item, ItemType itemType){
  if (item == NULL || itemType < 0 || itemType > COLLUM )
    return 0;
  item->type = itemType;
  return 1;
}
int setItemBbox(Item *item,float bbox[]){
  if (item == NULL || bbox == NULL)
    return 0;
//  if (coord[0] < -1.f || coord[0] > 1.f || coord[1] < -1.f || coord[1] > 1.f)
  //  return -1;
  item->bbox[0] = bbox[0];
  item->bbox[1] = bbox[1];
  item->bbox[2] = bbox[2];
  item->bbox[3] = bbox[3];
  return 1;
}

int setItemMargin(Item *item,float margin[]){
  if (item == NULL || margin == NULL)
    return 0;
//  if (coord[0] < -1.f || coord[0] > 1.f || coord[1] < -1.f || coord[1] > 1.f)
  //  return -1;
  item->margin[0] = margin[0];
  item->margin[1] = margin[1];
  item->margin[2] = margin[2];
  item->margin[3] = margin[3];
  return 1;
}

int setItemDescriptor(Item *item, Descriptor descriptor){
  if (item == NULL)
    return 0;
  //maybe some other control

  if (item->descriptor.name == NULL){
    item->descriptor.name = malloc(strlen(descriptor.name)* sizeof(char));
    strcpy(item->descriptor.name,descriptor.name);
  }else{
    free(item->descriptor.name);
    item->descriptor.name = malloc(strlen(descriptor.name)* sizeof(char));
    strcpy(item->descriptor.name,descriptor.name);
  }
  item->descriptor.font = descriptor.font;
  item->descriptor.fontSize = descriptor.fontSize;
  item->descriptor.color.r = descriptor.color.r;
  item->descriptor.color.g = descriptor.color.g;
  item->descriptor.color.b = descriptor.color.b;
  item->descriptor.color.a = descriptor.color.a;
  item->descriptor.action = descriptor.action;
  item->descriptor.bbox[0] = descriptor.bbox[0];
  item->descriptor.bbox[1] = descriptor.bbox[1];
  item->descriptor.bbox[2] = descriptor.bbox[2];
  item->descriptor.bbox[3] = descriptor.bbox[3];
  return 1;
}
int setItemMenu(Item *item, Menu *menu){
  if (item == NULL || menu == NULL)
    return 0;
  item->menu = menu;
  return 1;
}

//Build prototype
int addItemToMenu(Menu *menu, Item *item){
  if (menu == NULL || item == NULL)
    return 0;
  if (menu->size == MAX_MENU_SIZE)
    return -1;
  menu->item[menu->size] = item;
  menu->size++;
  return 1;
}

int calcMenu(Menu *menu){
  int i = 0;
  float menuWidth = 0.f;
  float menuHeight = 0.f;

  if (menu == NULL)
    return 0;
  for (i = 0; i < menu->size; i++) {
    ftglSetFontFaceSize(menu->item[i]->descriptor.font, menu->item[i]->descriptor.fontSize, 72);
    ftglGetFontBBox(menu->item[i]->descriptor.font,menu->item[i]->descriptor.name,strlen(menu->item[i]->descriptor.name),menu->item[i]->descriptor.bbox);
    if ( (menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0]) + menu->item[i]->margin[0]  >= menuWidth)
      menuWidth = menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0] + menu->item[i]->margin[0];
    menuHeight +=  menu->item[i]->descriptor.bbox[4]-menu->item[i]->descriptor.bbox[1] + menu->item[i]->margin[1];
    setItemBbox(menu->item[i], (float[]) {0.f,0.f,menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0], menu->item[i]->descriptor.bbox[4]-menu->item[i]->descriptor.bbox[1]}   );
  }
  setMenuBbox(menu,(float []){menu->bbox[0], menu->bbox[1], menuWidth, menuHeight});
  return 1;
}
