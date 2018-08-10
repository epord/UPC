#version 330 core
uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec2 vtexCoord;
out vec4 frontColor;

uniform vec3 minBoundingBox;
uniform vec3 maxBoundingBox;

void main()
{
    vec3 V = vertex;
    V.y *= -1;
    V.y -= maxBoundingBox.y - minBoundingBox.y;
	gl_Position = modelViewProjectionMatrix * vec4(V,1.0);
    vtexCoord = (4.0/1.0)*(vertex.xy+vertex.zx);
    frontColor = vec4(color, 1);
}

