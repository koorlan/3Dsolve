#include "camera.h"

Camera * cameraCreate ()
{
	Camera* camera = (Camera*) malloc (sizeof(Camera));
	
	return  camera ;
}

