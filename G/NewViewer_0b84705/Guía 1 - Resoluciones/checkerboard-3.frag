#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform float n = 8;

void main()
{
    float s = vtexCoord.s * n;
    float t = vtexCoord.t * n;
    if (fract(s) < 0.1 || fract(t) < 0.1) {
        fragColor = vec4(0, 0, 0, 1);
    } else {
        fragColor = vec4(0.8, 0.8, 0.8, 1);
    }
}
