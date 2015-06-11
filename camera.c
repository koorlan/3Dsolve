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

#include "camera.h"

Camera * cameraCreate ()
{
	Camera* camera = (Camera*) malloc (sizeof(Camera));

	return  camera ;
}

void cameraReset ( Camera* camera )
{
	vec3 vol_offset;
	vol_offset[0]=(app->snake->volume.max.x%2==0 ? app->snake->volume.max.x /2 - 0.5f : (app->snake->volume.max.x)/2);
	vol_offset[1]=(app->snake->volume.max.y%2==0 ? app->snake->volume.max.y /2 - 0.5f : (app->snake->volume.max.y)/2);
	vol_offset[2]=(app->snake->volume.max.z%2==0 ? app->snake->volume.max.z /2 - 0.5f : (app->snake->volume.max.z)/2);
	camera->eye[0] = 0.f;
	camera->eye[1] = 0.f;
	camera->eye[2] = 0.f;
	camera->target[0] = vol_offset[0];
	camera->target[1] = vol_offset[1];
	camera->target[2] = vol_offset[2];
	camera->up[0] = 0.f;
	camera->up[1] = 1.f;
	camera->up[2] = 0.f;
	camera->angle[0] = 0.f;
	camera->angle[1] = 0.5f;
	camera->fov = 1.6f;
	camera->distance = 4.f;
	camera->eye[0] = camera->target[0] + camera->distance
				* sin(camera->angle[0]) * cos(camera->angle[1]);
	camera->eye[2] = camera->target[2] + camera->distance
				* cos(camera->angle[0]) * cos(camera->angle[1]);
	camera->eye[1] = camera->target[1] + camera->distance
				* sin(camera->angle[1]);


}
