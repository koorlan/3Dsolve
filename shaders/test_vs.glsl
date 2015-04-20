#version 130
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 VP;
uniform mat4 W;

out vec3 color;

void main () {
  color = vertex_color;
  gl_Position = VP * W * vec4 (vertex_position, 1.0);
}
