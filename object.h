#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "linmath.h"
#include "log.h"

#define M_TRIS 0
#define M_QUADS 1
#define M_RGB 0
#define M_RGBA 1


typedef struct object_t
{
	int method;
	int nb_faces;
	GLuint vao_id;
	GLubyte * indices;
} Object;


Object * object_load(const char * file);
int object_destroy(Object * object);

#endif //OBJECT_H
