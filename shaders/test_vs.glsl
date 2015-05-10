#version 110
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 VP;
uniform mat4 W;

//varying vec3 color;

varying vec2 UV;

void main () {
  UV = vertex_uv;
  gl_Position = VP * W * vec4 (vertex_position, 1.0);
}
