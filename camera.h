#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>

#include "linmath.h"

typedef struct camera {
	vec3 eye;
	vec3 target;
	vec3 up;
	vec2 angle;
	float distance;
	float fov;
} Camera;

#include "application.h"

Camera* cameraCreate ();
void cameraReset ( Camera* camera );

#endif //CAMERA_H
