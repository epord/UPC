#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
in vec3 N[];
out vec4 gfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform float time;

void main( void )
{
    const float speed = 0.5;
    
    vec3 mean_N = vec3(0,0,0);
    for( int i = 0 ; i < 3 ; i++ )
    {
        mean_N += N[i];
    }
    mean_N /= 3;
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		gl_Position = modelViewProjectionMatrix
                        * (gl_in[i].gl_Position + speed * time * vec4(mean_N,0));
		EmitVertex();
	}
    EndPrimitive();
}
