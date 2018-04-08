#version 330 core

in vec4 frontColor;
in vec3 V; // in eye space
in vec3 norm;
out vec4 fragColor;


void main()
{
    vec3 Vx = dFdx(V);
    vec3 Vy = dFdy(V);
    vec3 N = normalize(cross(Vx, Vy));
    fragColor = frontColor * N.z;
}
