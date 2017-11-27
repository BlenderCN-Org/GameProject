#version 410

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;

layout(location = 0) out vec2 fragUv;

vec4 plane = vec4(0.0, 0.0, 0.0, 0.0);

void main()
{
	fragUv = vertexUv;
	gl_Position = vec4(vertexPos, 1.0);
	
}