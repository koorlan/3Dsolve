#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "log.h"

GLuint shader_load (char * file, GLenum type);
int shader_compile (GLuint shader_id);
GLuint shader_create_program(GLuint vertex_shader_id, GLuint fragment_shader_id);
int shader_destroy(GLuint shader_id);
int shader_program_destroy(GLuint shader_program_id, GLuint vs_id, GLuint fs_id);
void shader_print_info_log (GLuint shader_index);
void shader_print_program_info_log (GLuint shader_program);

#endif //SHADER_H
