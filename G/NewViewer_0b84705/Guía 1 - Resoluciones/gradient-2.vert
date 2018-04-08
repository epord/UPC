#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;


void main()
{
    vec3 N = normalize(normalMatrix * normal);
    
    vec3 red = vec3(1,0,0);
    vec3 yellow = vec3(1,1,0);
    vec3 green = vec3(0,1,0);
    vec3 cyan = vec3(0,1,1);
    vec3 blue = vec3(0,0,1);
    
    vec4 V_clip = modelViewProjectionMatrix * vec4(vertex, 1.0);
    vec3 V_NDS = V_clip.xyz / V_clip.w;
    float y = (V_clip.y / V_clip.w); // y in [-1; 1]
    y = ((y - (-1)) / 2) * 4; // y in [0; 4]
    vec3 final_color = mix(red, blue, y);
    
    if(y < 1.0)
        final_color = mix(red, yellow, fract(y));
    if(1.0 <= y && y < 2.0)
        final_color = mix(yellow, green, fract(y));
    if(2.0 <= y && y < 3.0)
        final_color = mix(green, cyan, fract(y));
    if(3.0 <= y && y < 4.0)
        final_color = mix(cyan, blue, fract(y));
    
    frontColor = vec4(final_color,1.0);
    vtexCoord = texCoord;
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
