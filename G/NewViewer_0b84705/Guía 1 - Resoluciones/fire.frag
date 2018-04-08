#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform float slice=0.1;
uniform float time;
uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;

void main()
{
    int frameNumber = int((time/slice));
    int modResult = int(mod(frameNumber,4));
    switch (modResult) {
        case 0:
            fragColor = texture(sampler0,vtexCoord);
            break;
        case 1:
//            fragColor = texture(sampler0,vtexCoord); Creo que el test tiene esto...
            fragColor = texture(sampler1,vtexCoord);
            break;
        case 2:
            fragColor = texture(sampler2,vtexCoord);
            break;
        case 3:
            fragColor = texture(sampler3,vtexCoord);
            break;
            
        default:
            fragColor = frontColor;
    }
    
}
