#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "log.h"

GLuint shaderLoad (char * file, GLenum type);
int shaderCompile (GLuint shader_id);
GLuint shaderCreateProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
int shaderProgramDestroy(GLuint shader_program_id, GLuint vs_id, GLuint fs_id);
void shaderPrintInfo (GLuint shader_index);
void shaderPrintProgramInfo (GLuint shader_program);

#endif //SHADER_H
