#version 330 core

in vec4 frontColor;
in vec4 gl_FragCoord;
out vec4 fragColor;

uniform int n = 2;

void main()
{
    if (int(gl_FragCoord.y-0.5) % n == 0)
        fragColor = frontColor;
    else
        discard;
}
