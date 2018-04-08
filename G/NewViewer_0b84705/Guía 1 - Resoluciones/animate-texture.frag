#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform float speed = 0.1;
uniform float time;
uniform sampler2D colorMap;

void main()
{
    vec2 st = vtexCoord;
    st += time * speed;
    fragColor = frontColor * texture(colorMap, st);
}
