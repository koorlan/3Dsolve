#version 110
#extension GL_ARB_explicit_attrib_location : enable

in vec3 color;
out vec4 frag_color;

void main () {
  frag_color = vec4 (color, 0.5);
}
