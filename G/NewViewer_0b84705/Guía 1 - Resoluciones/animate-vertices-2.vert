#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform float amplitude=0.1;
uniform float freq=1; //expressadaenHz
uniform float time;
uniform float PI = 3.1415926535;

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(1,1,1,1.0) * N.z;
    vtexCoord = texCoord;
    vec3 V = vertex;
    V += normalize(normal) * sin(time * 2 * PI * freq + PI * 2 * vtexCoord.s) * amplitude;
    gl_Position = modelViewProjectionMatrix * vec4(V, 1.0);
}
