#version 110

varying vec2 UV;

uniform sampler2D CurTex;
uniform float time;
uniform float alpha;


void main () {
  //gl_FragColor = texture2D( CurTex, UV ).rgba;
  gl_FragColor = vec4(texture2D( CurTex, UV ).rgb, alpha);
  //gl_FragColor = vec4 (1.0, 1.0, 1.0, 0.2);
}
