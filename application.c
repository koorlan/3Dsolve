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

    return app;
}

void applicationFindSnakes(Application* app)
{
    #ifdef _WIN32
        struct _finddata_t c_file;
        long   hFile;
        printf("Change to %s\n", path);
        if(_chdir("./Snakes"))
            printf("Unable to locate the directory: %s\n", path);
        else
            hFile = _findfirst("*.snake", &c_file);

        /* list the files... */
        printf("\nRDO HID SYS ARC  FILE         DATE %25c SIZE\n", ' ');
        printf("--- --- --- ---  ----         ---- %25c ----\n", ' ');
        printf((c_file.attrib & _A_RDONLY) ? " Y  " : " N  ");
        printf((c_file.attrib & _A_SYSTEM) ? " Y  " : " N  ");
        printf((c_file.attrib & _A_HIDDEN) ? " Y  " : " N  ");
        printf((c_file.attrib & _A_ARCH)   ? " Y  " : " N  ");

        printf(" %-12s %.24s  %9ld\n", c_file.name, ctime(&(c_file.time_write)), c_file.size);
        /* find the rest of the files */

        while(_findnext(hFile, &c_file) == 0)
        {
            printf((c_file.attrib & _A_RDONLY) ? " Y  " : " N  ");
            printf((c_file.attrib & _A_SYSTEM) ? " Y  " : " N  ");
            printf((c_file.attrib & _A_HIDDEN) ? " Y  " : " N  ");
            printf((c_file.attrib & _A_ARCH)   ? " Y  " : " N  ");
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
