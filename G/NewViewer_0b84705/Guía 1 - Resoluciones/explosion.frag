#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform sampler2D explosion;
uniform float time;

void main()
{
    vec2 st = vtexCoord;
//    initial texture position
    st.s = st.s/8;
    st.t = st.t/6 + 5.0/6.0;
    
    int frameNumber = int(time*30);
    st.s += frameNumber/8.0;
    st.t -= frameNumber/8/6.0;
    
    fragColor = texture(explosion, st)*texture(explosion, st).a;
}
