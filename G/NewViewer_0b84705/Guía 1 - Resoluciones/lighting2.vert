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

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matShininess;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular, lightPosition;


vec4 light(vec3 N, vec3 V, vec3 L);

void main()
{
    vec3 P = (modelViewMatrix * vec4(vertex.xyz, 1.0)).xyz;
    vec3 N = normalize(normalMatrix * normal);
    vec3 V = -P;
    vec3 L = lightPosition.xyz - P;
    frontColor = light(N, V, L);
    vtexCoord = texCoord;
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}

/*
 *  N is normal vector (eye space)
 *  V is vector from vertex to camera (eye space)
 *  L is vector to light source
 */
vec4 light(vec3 N, vec3 V, vec3 L) {
    N = normalize(N);
    V = normalize(V);
    L = normalize(L);
    
    float NdotL = max(0.0, dot(N, L));
    vec3 R = 2 * NdotL * N - L;
    float RdotV = max(0.0, dot(R, V));
    float Ispec = 0;
    if (NdotL > 0) Ispec = pow(RdotV, matShininess);
    
    return matAmbient * lightAmbient
         + matDiffuse * lightDiffuse * NdotL
         + matSpecular * lightSpecular * Ispec;
}
