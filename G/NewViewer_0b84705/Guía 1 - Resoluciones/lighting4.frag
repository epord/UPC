#version 330 core

in vec4 frontColor;
in vec3 N;
in vec3 pos;

out vec4 fragColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;

uniform float matShininess;


vec4 light(vec3 N, vec3 V, vec3 L);

void main()
{
    vec3 V = -pos;
    vec3 L = lightPosition.xyz - pos;
    fragColor = light(N, V, L);
}

/*  N is normal vector (eye space)
 *  V is vector to the camera
 *  L is vector to the light (eye space)
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

