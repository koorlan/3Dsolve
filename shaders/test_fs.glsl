#version 110
#extension GL_ARB_explicit_attrib_location : enable

in vec3 color;


void main () {
  gl_FragColor = vec4 (color, 1.0);
}
