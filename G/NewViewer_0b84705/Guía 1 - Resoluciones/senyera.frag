#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

void main()
{
    int s = abs(int(vtexCoord.s * 9));
    if ((s-s/9) % 2 == 0) { // se resta s/9 porque cada 9 lineas se invierte el patrón
        fragColor = vec4(1,1,0,1);
    } else {
        fragColor = vec4(1,0,0,1);
    }
}
