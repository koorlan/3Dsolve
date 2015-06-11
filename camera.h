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
