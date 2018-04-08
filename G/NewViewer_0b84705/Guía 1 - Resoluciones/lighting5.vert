#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

out vec3 N_eye;
out vec3 pos_eye;
out vec3 N_world;
out vec3 pos_world;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

uniform bool world;

// NOTE: objectSpace = worldSpace
void main()
{
    N_eye = normalMatrix * normal;
    pos_eye = (modelViewMatrix * vec4(vertex.xyz, 1.0)).xyz;
    N_world = normal;
    pos_world = vertex;
    
    vtexCoord = texCoord;
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}

