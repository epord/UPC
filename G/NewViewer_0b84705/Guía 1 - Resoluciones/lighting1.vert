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


vec4 blinn_phong_light(vec3 N, vec3 V, vec3 L);

void main() {
    vec3 P = (modelViewMatrix * vec4(vertex.xyz, 1.0)).xyz; // Vertex position in eye space
    vec3 N = normalMatrix * normal;
    vec3 V = vec3(0,0,1);
    vec3 L = (lightPosition.xyz - P);
    frontColor = blinn_phong_light(N, V, L);
    gl_Position = modelViewProjectionMatrix * vec4(vertex.xyz, 1.0);
}

/*  N is normal vector (eye space)
 *  V is vector from the vertex to the camera (eye space)
 *  L is vector to the light (eye space)
 */
vec4 blinn_phong_light(vec3 N, vec3 V, vec3 L) {
    N=normalize(N);
    V=normalize(V);
    L=normalize(L);
    vec3 H = normalize(V + L);
    float NdotL = max(0.0, dot(N,L));
    float NdotH = max(0.0, dot(N,H));
    float Ispec = 0;
    if (NdotL > 0) Ispec = pow(NdotH, matShininess);
    
    return matAmbient * lightAmbient
         + matDiffuse * lightDiffuse * NdotL
         + matSpecular * lightSpecular * Ispec;
}
