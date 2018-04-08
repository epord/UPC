#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform vec3 boundingBoxMin; // cantonada de la capsa englobant
uniform vec3 boundingBoxMax; // cantonada de la capsa englobant
uniform float time;
uniform bool eyespace;
uniform float PI = 3.1415926535;

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(color,1.0);
    vtexCoord = texCoord;
    
    float r = distance(boundingBoxMax, boundingBoxMin)/2.0;
    float y = vertex.y;
    if (eyespace) {
        y = (modelViewMatrix * vec4(vertex, 1.0)).y;
    }
    float d = (r/10)*y;
    vec3 V = vertex + normal * d* sin(time);
    
    gl_Position = modelViewProjectionMatrix * vec4(V, 1.0);
}
