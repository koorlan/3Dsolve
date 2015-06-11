/**
 * Projet application - 3DSolve
 * Version : 1.0
 *
 * Programme pour la résolution et la manipulation virtuelle de
 * casse-tête de type "Snake Cube"
 *
 * Auteurs :
 * 	- L.Aubry <lisa.aubry@insa-cvl.fr>
 * 	- A.Chazot <alban.chazot@insa-cvl.fr>
 * 	- K.Colas <korlan.colas@insa-cvl.fr>
 * 	- A.Gourd <anthony.gourd@insa-cvl.fr>
 *
 * Tuteur :
 * 	- P.Clemente <patrice.clemente@insa-cvl.fr>
 *
 * Date : 11 / 06 / 15
 *
 * INSA - Centre Val De Loire
 * 2014-2015
 * Promotion 2017
 *
 */

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
	GLuint vbo_points_id;
	GLuint vbo_uvs_id;
	GLubyte * indices;
} Object;


Object * objectLoad(const char * file);
int objectDestroy(Object * object);

#endif //OBJECT_H
