#version 330 core
out vec4 fragColor;


uniform float SIZE;
in vec2 vtexCoord;
in vec4 frontColor;

const int W = 2; // filter size: 2W*2W

void main()
{
    fragColor = frontColor;
    
}

