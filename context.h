#ifndef CONTEXT_H
#define CONTEXT_H

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
#include <math.h>

#include "renderer.h"
#include "log.h"
#include "shader.h"
#include "object.h"
#include "camera.h"
#include "snake.h"
#include "lodepng.h"
#include "player.h"

#define FULLSCREEN	0
#define DRESX		800
#define DRESY		600

#define M_RIGHT		0b00000001
#define M_LEFT		0b00000010
#define M_MOVE		0b00000100
#define M_ROLLF		0b00001000
#define M_ROLLB		0b00010000
#define M_LEFTONCE	0b00100000
#define M_RLEFTONCE	0b01000000
#define M_MID		0b10000000
#define M_NONE		0b00000000
#define MOUSE_SPEED	0.01f

#define K_UP		0b00000001
#define K_DN		0b00000010
#define K_LF		0b00000100
#define K_RT		0b00001000
#define K_RST		0b00010000
#define K_PGUP		0b00100000
#define K_ENTER		0b01000000

#define BHV_ROTATE	0b00000001
#define BHV_SPREAD	0b00000010

#define PM_PLAY		0
#define PM_RESOLVE	1

#define MAG_TRESHOLD	60
#define MAG_STEP	5

unsigned char key_flags;
unsigned char bhv_flags;
unsigned char mouse_flags;
float last_xpos;
float last_ypos;
float gxpos;
float gypos;
int resize_w;
int resize_h;


typedef struct context
{
	GLFWwindow* window;
	int screen_width;
	int screen_height;
	int running;
	pthread_t render_thread;
	GLuint volume_program;
	GLuint snake_program;
	GLuint picking_program;
	GLuint dwoodtex;
	GLuint lwoodtex;
	Object* cube_mesh;
	Object* square_mesh;
	Camera* camera;
	float ratio;
	Snake* snake;
	int drawpick;
	int playmode;
	int spread;
} Context;


void resizeCallback (GLFWwindow* window, int width, int height);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
int getInput ( Context* context );

Context* contextCreate ();
void contextInit ( Context* context );
void contextDestroy ( Context * context );

#endif //CONTEXT_H
