#version 110

varying vec2 UV;

uniform vec3 colorID;

void main () {
  //gl_FragColor = texture2D( CurTex, UV ).rgba;
  gl_FragColor = vec4 (colorID, 1.0);
}
