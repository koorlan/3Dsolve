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
    app->pathExplored = 0;
    app->calculTime = 0;

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
          if(dir->d_type == DT_REG && strstr(dir->d_name, ".snake") != NULL)
            i++;
        }
        //closedir(d);
        app->snakeNumber = i;
        i = 0;
        app->snakeNames = malloc(app->snakeNumber * sizeof(char*));
        rewinddir(d);
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_type == DT_REG && strstr(dir->d_name, ".snake") != NULL)
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
