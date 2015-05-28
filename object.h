#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "linmath.h"
#include "log.h"

#define M_TRIS 0
#define M_QUADS 1
#define M_OBJ 50
#define M_RGB 0
#define M_RGBA 1


typedef struct object
{
	int method;
	int nb_faces;
	GLuint vao_id;
	GLubyte * indices;
} Object;


Object * objectLoad(const char * file);
int objectDestroy(Object * object);

#endif //OBJECT_H
