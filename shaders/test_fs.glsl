#version 110
#extension GL_ARB_explicit_attrib_location : enable

in vec2 UV;

uniform sampler2D CurTex;

void main () {
  gl_FragColor = texture2D( CurTex, UV ).rgba;
  //gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0);
}
