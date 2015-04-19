#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>

#include "linmath.h"

typedef struct camera_t {
	vec3 eye;
	vec3 target;
	vec3 up;
	vec2 angle;
	float distance;
	float fov;
} Camera;


Camera* camera_create ();

#endif //CAMERA_H
