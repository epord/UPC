#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform vec2 Min=vec2(-1,-1);
uniform vec2 Max=vec2(1,1);

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(color,1.0);
    vec4 tCoord_clip = modelViewProjectionMatrix * vec4(texCoord,0, 1.0);
    vec4 tCoord_ndc = tCoord_clip / tCoord_clip.w;
    tCoord_ndc.x = (texCoord.x - Min.x)/(Max.x-Min.x) * 2-1;
    tCoord_ndc.y = (texCoord.y - Min.y)/(Max.y-Min.y) * 2-1;
    tCoord_clip = tCoord_ndc * tCoord_clip.w; 
    gl_Position = tCoord_clip;
}
