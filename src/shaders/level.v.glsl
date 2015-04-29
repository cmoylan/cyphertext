#version 430 core

layout(location = 0) in vec4 vPosition;
in vec2  vTexPosition;
out vec2 fTexPosition;

void
main()
{
    gl_Position = vPosition;
    fTexPosition = vTexPosition;
}