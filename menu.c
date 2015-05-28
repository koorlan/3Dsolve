#include "menu.h"

//Menu Prototype
int initMenu(Menu **menu){

  *menu = malloc(sizeof(Menu));
  if(*menu == NULL)
    return 0;
  (*menu)->type = COLUMN;
  memset((*menu)->margin, 0.f,4*sizeof(float));
  memset((*menu)->bbox, 0.f,4*sizeof(float));
  memset((*menu)->bboxRel, 0.f,4*sizeof(float));
  (*menu)->size = 0;
  (*menu)->selected = -1;
  (*menu)->item = malloc( MAX_MENU_SIZE * sizeof(Item)); // A voir si pas sizeof(*(*menu)->item)
  (*menu)->mutex = malloc( sizeof(pthread_mutex_t));
  memset((*menu)->scale, 1.f,3*sizeof(float));
  pthread_mutex_init((*menu)->mutex, NULL);

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

  memset((*item)->bbox, 0.f,4*sizeof(float));
  memset((*item)->margin, 0.f,4*sizeof(float));

  (*item)->descriptor.name = NULL;
  (*item)->descriptor.font = NULL;
  (*item)->descriptor.fontSize = 0;
  (*item)->descriptor.maxFontSize = 0;
  (*item)->descriptor.minFontSize = 0;
  (*item)->descriptor.color.r = 0.f;
  (*item)->descriptor.color.g = 0.f;
  (*item)->descriptor.color.b = 0.f;
  (*item)->descriptor.color.a = 0.f;
  (*item)->descriptor.action = NONE;
  memset((*item)->descriptor.bbox, 0.f,6*sizeof(float));

  (*item)->menu = NULL;
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
  descriptor->minFontSize = item->descriptor.minFontSize ;
  descriptor->maxFontSize = item->descriptor.maxFontSize ;
  descriptor->color.r = item->descriptor.color.r ;
  descriptor->color.g = item->descriptor.color.g ;
  descriptor->color.b = item->descriptor.color.b ;
  descriptor->color.a = item->descriptor.color.a ;
  descriptor->action = item->descriptor.action ;
  descriptor->bbox[0] = item->descriptor.bbox[0];
  descriptor->bbox[1] = item->descriptor.bbox[1];
  descriptor->bbox[2] = item->descriptor.bbox[2];
  descriptor->bbox[3] = item->descriptor.bbox[3];
  descriptor->bbox[4] = item->descriptor.bbox[4];
  descriptor->bbox[5] = item->descriptor.bbox[5];


  return 1;
}
int getItemMenu(Item *item, Menu *menu){
  if( item == NULL || menu == NULL)
    return 0;
  if ( item->menu == NULL )
    return -1; // Only ROW and COLLUM can have a pointer to a menu.
  menu = item->menu;
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
  item->descriptor.fontSize = (descriptor.fontSize < 1 ) ? DEFAULT_MAX_FONT_SIZE : descriptor.fontSize ;
  item->descriptor.maxFontSize = (descriptor.maxFontSize < 1 ) ?  DEFAULT_MAX_FONT_SIZE : descriptor.maxFontSize ;
  item->descriptor.minFontSize = (descriptor.minFontSize < 1 ) ?  DEFAULT_MIN_FONT_SIZE : descriptor.minFontSize ;
  item->descriptor.color.r = descriptor.color.r;
  item->descriptor.color.g = descriptor.color.g;
  item->descriptor.color.b = descriptor.color.b;
  item->descriptor.color.a = descriptor.color.a;
  item->descriptor.action = descriptor.action;
  item->descriptor.bbox[0] = descriptor.bbox[0];
  item->descriptor.bbox[1] = descriptor.bbox[1];
  item->descriptor.bbox[2] = descriptor.bbox[2];
  item->descriptor.bbox[3] = descriptor.bbox[3];
  item->descriptor.bbox[4] = descriptor.bbox[4];
  item->descriptor.bbox[5] = descriptor.bbox[5];
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
    if (ftglGetFontFaceSize(menu->item[i]->descriptor.font) != menu->item[i]->descriptor.fontSize 	){
      ftglSetFontFaceSize(menu->item[i]->descriptor.font, menu->item[i]->descriptor.fontSize,72);
      ftglSetFontCharMap(menu->item[i]->descriptor.font, ft_encoding_unicode);
      }
    ftglGetFontBBox(menu->item[i]->descriptor.font,menu->item[i]->descriptor.name,strlen(menu->item[i]->descriptor.name),menu->item[i]->descriptor.bbox);
    if ( (menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0]) + menu->item[i]->margin[0] + menu->item[i]->margin[2]  >= menuWidth)
      menuWidth = menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0] + menu->item[i]->margin[0]  + menu->item[i]->margin[2];
    menuHeight +=  menu->item[i]->descriptor.bbox[4]-menu->item[i]->descriptor.bbox[1] + menu->item[i]->margin[1] + menu->item[i]->margin[3];
    setItemBbox(menu->item[i], (float[]) {0.f,0.f,menu->item[i]->descriptor.bbox[3]-menu->item[i]->descriptor.bbox[0],menu->item[i]->descriptor.bbox[4]-menu->item[i]->descriptor.bbox[1]}   );
  }
  setMenuBbox(menu,(float []){menu->bbox[0], menu->bbox[1], menuWidth, menuHeight});
  return 1;
}

int reshapeMenu(Menu *menu, int width, int height){
  int reduced = 0,
      increased = 0;

  if ( menu == NULL)
    return 0;
  calcMenu(menu);
  while( !reduced || !increased){
    if( !increased && ( ((menu->bbox[2] - menu->bbox[0] >= width) ||  (menu->bbox[3] - menu->bbox[1] >= height)) || increaseMenu(menu) == -1 )){
      increased = 1;
    } else if ( !reduced && ( !((menu->bbox[2] - menu->bbox[0] >= width) ||  (menu->bbox[3] - menu->bbox[1] >= height)) || (reduceMenu(menu) == -1) ) ){
        reduced = 1;
      }
  }

  return 1;
  return -1;
}

int reduceMenu(Menu *menu){
  if ( menu == NULL)
    return 0;
  int i = 0,
      limit = 0;
  for (i = 0; i < menu->size ; i++) {
    menu->item[i]->descriptor.fontSize -= 1;
    if (menu->item[i]->descriptor.fontSize <= menu->item[i]->descriptor.minFontSize){
      menu->item[i]->descriptor.fontSize = menu->item[i]->descriptor.minFontSize;
      limit = 1;
      continue;
    }else {
      menu->item[i]->margin[0] = menu->item[i]->margin[0] * 0.90f ;
      menu->item[i]->margin[1] = menu->item[i]->margin[1] * 0.90f ;
      menu->item[i]->margin[2] = menu->item[i]->margin[2] * 0.90f ;
      menu->item[i]->margin[3] = menu->item[i]->margin[3] * 0.90f ;
    }
  }
  calcMenu(menu);
  if(limit)
    return -1;
  return 1;
}

int increaseMenu(Menu *menu){
  if ( menu == NULL)
    return 0;
  int i = 0,
      limit = 0;

  for (i = 0; i < menu->size ; i++) {
      menu->item[i]->descriptor.fontSize += 1;
    if (menu->item[i]->descriptor.fontSize >= menu->item[i]->descriptor.maxFontSize){
      menu->item[i]->descriptor.fontSize = menu->item[i]->descriptor.maxFontSize;
      limit = 1;
      continue;
    } else{
      menu->item[i]->margin[0] = menu->item[i]->margin[0] * 1.10f ;
      menu->item[i]->margin[1] = menu->item[i]->margin[1] * 1.10f ;
      menu->item[i]->margin[2] = menu->item[i]->margin[2] * 1.10f ;
      menu->item[i]->margin[3] = menu->item[i]->margin[3] * 1.10f ;
    }
  }
  calcMenu(menu);
  if(limit)
    return -1;
  return 1;
}


int testMenuMesh(Menu *menu,int width, int height){
  int i,j;
  //float ratio = (float)width / (float)height;
  float mW = (menu->bbox[2] - menu->bbox[0] )/width ;
  float mH = (menu->bbox[3] - menu->bbox[1]) /height;
  logWrite("[MENUTEST] menuW = %f , menuH = %f",mW,mH);
  float * points = (float *) malloc (6 * 3 * sizeof(float));
  float * uvs = malloc (6 * 2 * sizeof(float));
  //set background menu vertex
  points = (float []) {
    mW/width,-mH/height,0.f,
    -mW/width,-mH/height,0.f,
    mW/width,mH/height,0.f,

    mW/width,mH/height,0.f,
    -mW/width,-mH/height,0.f,
    -mW/width,mH/height,0.f
  };

  menu->bboxRel[0] = -mW;
  menu->bboxRel[1] = mH ;
  menu->bboxRel[2] = mW ;
  menu->bboxRel[3] = -mH;


  //Generate each mesh for items
  float acc =0.f ;
  for (i = 0; i < menu->size ; i++){
    //menu->item[i]->mesh = objectLoad("stc/square.stc");
    mW = (menu->item[i]->bbox[2] - menu->item[i]->bbox[0])/width ;
    mH = (menu->item[i]->bbox[3] - menu->item[i]->bbox[1])/height;

    menu->item[i]->bboxRel[0] = -mW;
    menu->item[i]->bboxRel[1] = mH ;
    menu->item[i]->bboxRel[2] = mW ;
    menu->item[i]->bboxRel[3] = -mH;

    menu->item[i]->marginRel[0] = (menu->item[i]->margin[0]*menu->bboxRel[2])/2*((menu->bbox[2] - menu->bbox[0] )/width );
    menu->item[i]->marginRel[1] = (menu->item[i]->margin[1]*4*menu->bbox[1])/height;
    menu->item[i]->marginRel[2] = (menu->item[i]->margin[2]*menu->bboxRel[2])/2*((menu->bbox[2] - menu->bbox[0] )/width );
    menu->item[i]->marginRel[3] = (menu->item[i]->margin[3]*4*menu->bbox[1])/height;
  }
  return 1;
}

int menuCalcRelativeCoord(Menu *childMenu,Menu *parentMenu){

}
