#ifndef CONTEXT_H
#define CONTEXT_H

#define GLFW_INCLUDE_GLU
#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <pthread.h>
#include <math.h>

#include "renderer.h"
#include "log.h"
#include "shader.h"
#include "object.h"
#include "camera.h"
#include "snake.h"

#define FULLSCREEN 0
#define DRESX 800
#define DRESY 600
#define M_RIGHT	0b00000001
#define M_LEFT	0b00000010

int keys;
unsigned char mouse_flags;
float last_xpos;
float last_ypos;
double xpos;
double ypos;


typedef struct context_t
{
	GLFWwindow* window;
	int screen_width;
	int screen_height;
	int running;
	pthread_t render_thread;
	GLuint shader_program;
	Object* dcube_mesh;
	Object* lcube_mesh;
	Camera* camera;
	float ratio;
	Snake* snake;
} Context;


void resizeCallback (GLFWwindow* window, int width, int height);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
int getInput ( Context* context );

Context* context_create ();
void context_init ( Context* context );
void context_destroy ( Context * context );

#endif //CONTEXT_H
