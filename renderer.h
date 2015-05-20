#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_GLU
#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <pthread.h>
#include <time.h>
#include <math.h>

#include "camera.h"
#include "context.h"
#include "log.h"
#include "object.h"
#include "snake.h"

#define F_NEAR 0.1f
#define F_FAR 1000.0f

#define abs(X) ((X)<0?-(X):(X))

void* renderer ( void *arg );
void dir2vec ( Dir dir, vec3 vec );
void flatten ( vec3* cubes, int cubesNb, Unit* layout  );

#endif //RENDERERS_H
