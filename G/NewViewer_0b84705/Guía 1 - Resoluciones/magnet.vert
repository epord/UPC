#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrixInverse;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;
uniform float n = 4;

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(1,1,1,1) * N.z;
    vtexCoord = texCoord;
    
    vec3 F = (modelViewMatrixInverse * lightPosition).xyz; // light position in object space
    vec3 V = vertex; // in object space
    float d = distance(V, F);
    float w = clamp(pow(1/d, n), 0, 1);
    vec3 V2 = (1.0-w)*V + w*F; // new vertex position in object space
    gl_Position = modelViewProjectionMatrix * vec4(V2, 1.0);
}
