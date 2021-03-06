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

#include "application.h"

Application* applicationCreate()
{
    Application* app = malloc(sizeof(Application));
    if(app == NULL)
        return NULL;

    app->snakeNames = NULL;
    app->snakeNumber = 0;
    app->loadedSnake = 0;
    app->snake = NULL;
    app->player = NULL;
    app->solver = NULL;
    app->running = 1;
    app->state = AS_HOME;
    app->itemSelected = -1;
    app->buttonPushed = -1;

    return app;
}

void applicationDestroy(Application* app)
{
    int i;
    for(i=0; i < app->snakeNumber; i++)
        free(app->snakeNames[i]);
    free(app->snakeNames);
    snakeDestroy(app->snake, 1);

    free(app);
}

void applicationFindSnakes(Application* app)
{
    int i = 0;
    int length;
    DIR *d;
    struct dirent *dir;
    d = opendir("./Snakes");
    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
          if(strstr(dir->d_name, ".snake") != NULL)
            i++;
        }
        //closedir(d);
        app->snakeNumber = i;
        i = 0;
        app->snakeNames = malloc(app->snakeNumber * sizeof(char*));
        rewinddir(d);
        while ((dir = readdir(d)) != NULL)
        {
            if(strstr(dir->d_name, ".snake") != NULL)
            {
              length = strlen(dir->d_name) + 1;
              app->snakeNames[i] = malloc(length * sizeof(char));
              strncpy(app->snakeNames[i], dir->d_name, length);
              i++;
            }
        }
        closedir(d);
    }

    // DEBUG
    for(i = 0; i < app->snakeNumber; i++)
        logWrite("Snake n°%d : %s\n", i, app->snakeNames[i]);
}

int applicationInitMainMenu(Application *app,int width, int height){
  //Menu test;
  int i;
  Item *tmpitem;

  //init the first menu
  initMenu(&(app->menu));
  app->menu->type = COLUMN;
  //app->menu->item[0]->menu
  setMenuMargin(app->menu,(float []) {0.02f, 0.02f, 0.02f, 0.02f} );
    for ( i = 0;  i <3; i++) {
      initItem(&(tmpitem));
      //setItemStartCoord(tmpitem,(float[2]){-1.f+0.2f*i,1.f-0.2f*i});
      setItemMargin(tmpitem,(float[]){5.f,10.f,10.f,10.f});
      switch (i){
        case 0:
          setItemDescriptor(tmpitem,(struct Descriptor)
            {	.name="App",
              .font=LoadFont("fonts/Tahoma.ttf"),
              .fontSize = 20,
              .minFontSize = DEFAULT_MIN_FONT_SIZE,
              .maxFontSize = DEFAULT_MAX_FONT_SIZE,
              .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
              .action= MENU});
          break;
        case 1:
          setItemDescriptor(tmpitem,(struct Descriptor)
            {	.name="Load Snake",
              .font=LoadFont("fonts/Tahoma.ttf"),
              .fontSize = 20,
              .minFontSize = DEFAULT_MIN_FONT_SIZE,
              .maxFontSize = DEFAULT_MAX_FONT_SIZE,
              .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
              .action= MENU});
          break;
        case 2:
          setItemDescriptor(tmpitem,(struct Descriptor)
            {	.name="Solution",
              .font=LoadFont("fonts/Tahoma.ttf"),
              .fontSize = 20,
              .minFontSize = DEFAULT_MIN_FONT_SIZE,
              .maxFontSize = DEFAULT_MAX_FONT_SIZE,
              .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
              .action= MENU});
          break;
      }
      ftglSetFontFaceSize(tmpitem->descriptor.font,tmpitem->descriptor.fontSize,72);
      addItemToMenu(app->menu,tmpitem);
    }
    calcMenu(app->menu);
    app->menu->state = OPEN;
    app->menu->mesh = objectLoad("stc/menu.stc");
    calcMenuMesh(app->menu,width,height);

  //APP menu
    initMenu(&(app->menu->item[0]->menu));
    app->menu->item[0]->menu->type = COLUMN;
    //setMenuMargin(app->menu->item[0]->menu,(float []) {0.0f, 0.0f, 0.0f, 0.0f} );
    for ( i = 0;  i <4; i++) {
      initItem(&(tmpitem));
      //setItemStartCoord(tmpitem,(float[2]){-1.f+0.2f*i,1.f-0.2f*i});
      setItemMargin(tmpitem,(float[]){5.f,10.f,10.f,10.f});
      switch (i){
        case 0:
          setItemDescriptor(tmpitem,(struct Descriptor)
            {	.name="Quitter",
              .font=LoadFont("fonts/Tahoma.ttf"),
              .fontSize = 20,
              .minFontSize = DEFAULT_MIN_FONT_SIZE,
              .maxFontSize = DEFAULT_MAX_FONT_SIZE,
              .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
              .action= EXIT});
          break;
        case 1:
          setItemDescriptor(tmpitem,(struct Descriptor)
            {	.name="Back",
              .font=LoadFont("fonts/Tahoma.ttf"),
              .fontSize = 20,
              .minFontSize = DEFAULT_MIN_FONT_SIZE,
              .maxFontSize = DEFAULT_MAX_FONT_SIZE,
              .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
              .action= BACKAPP});
          break;
        case 2:
          setItemDescriptor(tmpitem,(struct Descriptor)
          {	.name="Aide",
          .font=LoadFont("fonts/Tahoma.ttf"),
          .fontSize = 20,
          .minFontSize = DEFAULT_MIN_FONT_SIZE,
          .maxFontSize = DEFAULT_MAX_FONT_SIZE,
          .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
          .action= HELP});
        break;
        case 3:
          setItemDescriptor(tmpitem,(struct Descriptor)
          {	.name="A propos",
          .font=LoadFont("fonts/Tahoma.ttf"),
          .fontSize = 20,
          .minFontSize = DEFAULT_MIN_FONT_SIZE,
          .maxFontSize = DEFAULT_MAX_FONT_SIZE,
          .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
          .action= ABOUT});
        break;
      }
      ftglSetFontFaceSize(tmpitem->descriptor.font,tmpitem->descriptor.fontSize,72);
      addItemToMenu(app->menu->item[0]->menu,tmpitem);
    }
    app->menu->item[0]->menu->state = CLOSE;
    app->menu->item[0]->menu->mesh = objectLoad("stc/menu.stc");
    calcMenu(app->menu->item[0]->menu);
    calcMenuMesh(app->menu->item[0]->menu,width,height);

    //Snake LOAD
    initMenu(&(app->menu->item[1]->menu));
    app->menu->item[1]->menu->type = COLUMN;
    for (i=0 ; i<app->snakeNumber; i++){
      initItem(&(tmpitem));
      setItemMargin(tmpitem,(float[]){5.f,10.f,10.f,10.f});
      setItemDescriptor(tmpitem,(struct Descriptor)
      {	.name=app->snakeNames[i],
        .font=LoadFont("fonts/Tahoma.ttf"),
        .fontSize = 20,
        .minFontSize = DEFAULT_MIN_FONT_SIZE,
        .maxFontSize = DEFAULT_MAX_FONT_SIZE,
        .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
        .action= LOADSNAKE});

      ftglSetFontFaceSize(tmpitem->descriptor.font,tmpitem->descriptor.fontSize,72);
      addItemToMenu(app->menu->item[1]->menu,tmpitem);
    }
    app->menu->item[1]->menu->state = CLOSE;
    app->menu->item[1]->menu->mesh = objectLoad("stc/menu.stc");
    calcMenu(app->menu->item[1]->menu);
    calcMenuMesh(app->menu->item[1]->menu,width,height);

    //Snake Solution
    initMenu(&(app->menu->item[2]->menu));
    app->menu->item[2]->menu->type = COLUMN;
    FTGLfont *itemFont = LoadFont("fonts/Tahoma.ttf");
    for (i=0 ; i<MAX_MENU_SIZE; i++){
      initItem(&(tmpitem));
      setItemMargin(tmpitem,(float[]){5.f,10.f,10.f,10.f});

      char buf[255];
      char snakeSolution[255] = "solution n°\0";
      sprintf(buf,"%d",i+1);
      strcat(snakeSolution,buf);
      setItemDescriptor(tmpitem,(struct Descriptor)
      {	.name=snakeSolution,
        .font=itemFont,
        .fontSize = 20,
        .minFontSize = DEFAULT_MIN_FONT_SIZE,
        .maxFontSize = DEFAULT_MAX_FONT_SIZE,
        .color=(struct Color){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f},
        .action= LOADSOL});

    //  ftglSetFontFaceSize(tmpitem->descriptor.font,tmpitem->descriptor.fontSize,72);
      addItemToMenu(app->menu->item[2]->menu,tmpitem);
    }

    app->menu->item[2]->menu->state = CLOSE;
    app->menu->item[2]->menu->mesh = objectLoad("stc/menu.stc");
    calcMenu(app->menu->item[2]->menu);
    calcMenuMesh(app->menu->item[2]->menu,width,height);
    app->menu->item[2]->menu->size = app->snake->solutions->size;
    calcMenu(app->menu->item[2]->menu);
    calcMenuMesh(app->menu->item[2]->menu,width,height);

    app->menuDepth =1;
    return 1;
}
