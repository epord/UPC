#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

void main()
{
    int s = abs(int(ceil(vtexCoord.s * 8)));
    int t = abs(int(ceil(vtexCoord.t * 8)));
    if ((s % 2 == 0 && t % 2 == 0)
        || (s % 2 == 1 && t % 2 == 1)) {
        fragColor = vec4(0.8, 0.8, 0.8, 1);
    } else {
        fragColor = vec4(0, 0, 0, 1);
    }
}
