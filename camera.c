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
