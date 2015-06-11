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
