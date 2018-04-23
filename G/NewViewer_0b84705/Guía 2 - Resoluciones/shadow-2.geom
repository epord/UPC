#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

uniform mat4 modelViewProjectionMatrix;
uniform vec3 boundingBoxMin;
uniform vec3 boundingBoxMax;

in vec4 vfrontColor[];
out vec4 gfrontColor;

void main( void )
{
    float Y = boundingBoxMin.y;
    
    if (gl_PrimitiveIDIn == 0) {
        float R = distance(boundingBoxMin,boundingBoxMax) / 2;
        vec3 C = vec3(boundingBoxMin.x + (boundingBoxMax.x - boundingBoxMin.x) / 2,
                      boundingBoxMin.y + (boundingBoxMax.y - boundingBoxMin.y) / 2,
                      boundingBoxMin.z + (boundingBoxMax.z - boundingBoxMin.z) / 2);
        gfrontColor = vec4(0,1,1,1);
        gl_Position = modelViewProjectionMatrix * vec4(C.x + R, Y-0.01, C.z + R, 1);
        EmitVertex();
        gl_Position = modelViewProjectionMatrix * vec4(C.x + R, Y-0.01, C.z - R, 1);
        EmitVertex();
        gl_Position = modelViewProjectionMatrix * vec4(C.x - R, Y-0.01, C.z + R, 1);
        EmitVertex();
        EndPrimitive();
        
        gl_Position = modelViewProjectionMatrix * vec4(C.x - R, Y-0.01, C.z - R, 1);
        EmitVertex();
        gl_Position = modelViewProjectionMatrix * vec4(C.x + R, Y-0.01, C.z - R, 1);
        EmitVertex();
        gl_Position = modelViewProjectionMatrix * vec4(C.x - R, Y-0.01, C.z + R, 1);
        EmitVertex();
        EndPrimitive();
    }
    
    for( int i = 0 ; i < 3 ; i++ )
    {
        gfrontColor = vfrontColor[i];
        gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();

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
