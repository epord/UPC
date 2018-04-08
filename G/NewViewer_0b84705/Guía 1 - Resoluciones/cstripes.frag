#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform int nstripes = 16;
uniform vec2 origin = vec2(0,0);

void main()
{
    float d = distance(vtexCoord, origin) * nstripes;
    if (int(d) % 2 == 0) {
        fragColor = vec4(1,0,0,1);
    } else {
        fragColor = vec4(1,1,0,1);
    }
}
