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

vec4 light(vec3 N, vec3 V, vec3 L, vec4 color);

void main()
{
    vec3 V = (modelViewMatrixInverse * vec4(0,0,0, 1)).xyz - pos;
    vec4 X_LIGHT = light(N, V, vec3(1,0,0), vec4(1,0,0,1));
    vec4 Y_LIGHT = light(N, V, vec3(0,1,0), vec4(0,1,0,1));
    vec4 Z_LIGHT = light(N, V, vec3(0,0,1), vec4(0,0,1,1));
    fragColor = X_LIGHT + Y_LIGHT + Z_LIGHT;
}

vec4 light(vec3 N, vec3 V, vec3 L, vec4 color)
{ N=normalize(N); V=normalize(V); L=normalize(L); vec3 R = normalize( 2.0*dot(N,L)*N-L );
    float NdotL = max( 0.0, dot( N,L ) );
    float RdotV = max( 0.0, dot( R,V ) );
    float Idiff = NdotL;
    float Ispec = 0;
    if (NdotL>0) Ispec=pow( RdotV, matShininess );
    return
    matDiffuse  * color * Idiff+
    matSpecular * lightSpecular * Ispec;
}
