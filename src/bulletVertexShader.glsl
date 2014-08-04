#version 150 core

in vec2 position;
in vec2 texcoord;
out vec2 Texcoord;
uniform mat4 trans;

void main() {
  gl_Position = trans * vec4(position.x, position.y, 0.0, 8.0);
  Texcoord = texcoord;
}
