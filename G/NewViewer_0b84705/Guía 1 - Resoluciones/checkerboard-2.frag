#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform float n = 8;

void main()
{
    int s = abs(int(ceil(vtexCoord.s * n)));
    int t = abs(int(ceil(vtexCoord.t * n)));
    if ((s % 2 == 0 && t % 2 == 0)
        || (s % 2 == 1 && t % 2 == 1)) {
        fragColor = vec4(0.8, 0.8, 0.8, 1);
    } else {
        fragColor = vec4(0, 0, 0, 1);
    }
}
