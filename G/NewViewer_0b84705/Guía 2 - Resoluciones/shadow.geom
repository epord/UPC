#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

uniform mat4 modelViewProjectionMatrix;
uniform vec3 boundingBoxMin;

in vec4 vfrontColor[];
out vec4 gfrontColor;

void main( void )
{
    for( int i = 0 ; i < 3 ; i++ )
    {
        gfrontColor = vfrontColor[i];
        gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
    
    float Y = boundingBoxMin.y;
    for( int i = 0 ; i < 3 ; i++ )
    {
        gfrontColor = vec4(0,0,0,1);
        vec4 pos = gl_in[i].gl_Position;
        
        pos.y = Y;
        
        gl_Position = modelViewProjectionMatrix * pos;
        
        EmitVertex();
    }
    EndPrimitive();
}
