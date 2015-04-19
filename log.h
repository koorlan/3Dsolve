#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#define LOG_FILE "snake.log"

int log_start ();
int log_write (const char* message, ...);
int log_warning (const char* message, ...);
int log_error (const char* message, ...);
void log_gl_params ();

#endif //LOG_H
