#version 150 core

in vec4 coord;
out vec2 Texcoord;

void main() {
  gl_Position = vec4(coord.xy, 0.0, 1.0);
  Texcoord = coord.zw;
}
