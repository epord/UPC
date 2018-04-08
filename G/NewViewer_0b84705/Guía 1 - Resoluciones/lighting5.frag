#version 330 core

in vec4 frontColor;

in vec3 N_eye;
in vec3 pos_eye;
in vec3 N_world;
in vec3 pos_world;

out vec4 fragColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 modelMatrixInverse;
uniform mat4 viewMatrixInverse;
uniform mat4 projectionMatrixInverse;
uniform mat4 modelViewMatrixInverse;
uniform mat4 modelViewProjectionMatrixInverse;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;

uniform float matShininess;

uniform bool world;

vec4 light(vec3 N, vec3 V, vec3 L);

void main()
{
    if (!world) {
        // OK
        vec3 V = -pos_eye;
        vec3 L = lightPosition.xyz - pos_eye;
        fragColor = light(N_eye, V, L);
    } else {
        // NOT OK
        vec3 V = (viewMatrixInverse * vec4(-pos_eye, 0)).xyz;
        vec3 L = (viewMatrixInverse * vec4(lightPosition.xyz - pos_eye, 0)).xyz;
        fragColor = light(N_world, V, L);
    }
}

/*  N is normal vector
 *  V is vector to the camera
 *  L is vector to the light
 */
vec4 light(vec3 N, vec3 V, vec3 L)
{ N=normalize(N); V=normalize(V); L=normalize(L); vec3 R = normalize( 2.0*dot(N,L)*N-L );
    float NdotL = max( 0.0, dot( N,L ) );
    float RdotV = max( 0.0, dot( R,V ) );
    float Idiff = NdotL;
    float Ispec = 0;
    if (NdotL>0) Ispec=pow( RdotV, matShininess );
    return
    matAmbient  * lightAmbient +
    matDiffuse  * lightDiffuse * Idiff+
    matSpecular * lightSpecular * Ispec;
}

