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

void applicationFindSnakes(Application* app)
{
    #ifdef _WIN32
        char* path = ".\\Snakes\\";
        struct _finddata_t c_file;
        long   hFile;
        printf("Change to %s\n", path);
        if(_chdir(path))
            printf("Unable to locate the directory: %s\n", path);
        else
            hFile = _findfirst("*.snake", &c_file);

        printf("File name %-12s\n", c_file.name);


        /* find the rest of the files */
        while(_findnext(hFile, &c_file) == 0)
        {
            printf(" %-12s %.24s  %9ld\n", c_file.name, ctime(&(c_file.time_write)), c_file.size);
        }
        _findclose(hFile);
    #else
        DIR *d;
        struct dirent *dir;
        d = opendir("./Snakes");
        if(d)
        {
          while ((dir = readdir(d)) != NULL)
          {
              if(dir->d_type == DT_REG && strstr(dir->d_name, ".snake") != NULL)
                app->snakeNumber++;
          }
          closedir(d);
        }
        d = opendir("./Snakes");
        int i = 0;
        int length;
        app->snakeNames = malloc(app->snakeNumber * sizeof(char*));
        if(d)
        {
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
        {
            printf("Snake n°%d : %s\n", i, app->snakeNames[i]);
        }
    #endif
}
