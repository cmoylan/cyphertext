#version 430 core

in vec2 fTexPosition;

uniform sampler2D tex;

out vec4 outColor;

void
main()
{
    //outColor = vec4(fTexPosition.xy, 1.0, 1.0);
    outColor = texture(tex, fTexPosition);
}
