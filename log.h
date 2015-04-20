#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#define LOG_FILE "snake.log"

int logStart ();
int logWrite (const char* message, ...);
int logWarning (const char* message, ...);
int logError (const char* message, ...);
void logGLParams ();

#endif //LOG_H
