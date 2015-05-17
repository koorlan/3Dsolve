#version 110
attribute vec3 vertex_position;
attribute vec2 vertex_uv;

uniform mat4 VP;
uniform mat4 W;

varying vec2 UV;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main () {
  UV = vertex_uv;
  gl_Position = VP * W * vec4 (vertex_position, 1.0);
}
