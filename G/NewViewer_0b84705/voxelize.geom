#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
out vec4 gfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform float step;

void main( void )
{
    vec3 bari = vec3(0,0,0);
    for( int i = 0 ; i < 3 ; i++ )
    {
        bari += gl_in[i].gl_Position.xyz;
    }
    bari /= 3;
    bari.x = round(bari.x);
    bari.y = round(bari.y);
    bari.z = round(bari.z);
    for( int i = 0 ; i < 3 ; i++ )
    {
		gfrontColor = vfrontColor[i];
		gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}
    EndPrimitive();
}
