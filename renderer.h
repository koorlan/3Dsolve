#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_GLU
#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "camera.h"
#include "context.h"
#include "log.h"
#include "object.h"

#define F_NEAR 0.1f
#define F_FAR 1000.0f

#define abs(X) ((X)<0?-(X):(X))

void* renderer ( void *arg );

#endif //RENDERERS_H
