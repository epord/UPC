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

uniform vec4 lightPosition; // Already in eye space

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;

uniform float matShininess;


vec4 blinn_phong_light(vec3 N, vec3 V, vec3 L);

void main()
{
    vec3 V = vec3(0,0,1);
    vec3 L = lightPosition.xyz - pos;
    fragColor = blinn_phong_light(N, V, L);
}

/*  N is normal vector (eye space)
 *  V is vector to the camera
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
