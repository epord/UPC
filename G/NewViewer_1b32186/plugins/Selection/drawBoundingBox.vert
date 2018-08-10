#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

uniform mat4 modelViewProjectionMatrix;
uniform vec4 minBoundingBox;
uniform vec4 maxBoundingBox;

void main()
{
// Centering base bounding box
    vec3 V = vertex - vec3(0.5, 0.5, 0.5);

// Rescaling
    V.x *= (maxBoundingBox.x - minBoundingBox.x);
    V.y *= (maxBoundingBox.y - minBoundingBox.y);
    V.z *= (maxBoundingBox.z - minBoundingBox.z);

// Move bounding box to center of object
    vec3 middle = vec3((minBoundingBox.x + maxBoundingBox.x)/2,
                       (minBoundingBox.y + maxBoundingBox.y)/2,
                       (minBoundingBox.z + maxBoundingBox.z)/2);
    V += middle;
    
    gl_Position = modelViewProjectionMatrix * vec4(V, 1.0);
}
