#version 330 core

in vec4 frontColor;
out vec4 fragColor;
in vec4 vertex_clip;

uniform float time;

void main()
{
    float fragCoord_cs = vertex_clip.x / vertex_clip.w;
    if (fragCoord_cs > time - 1)
        discard;
    fragColor = frontColor;
}
