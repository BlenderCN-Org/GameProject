#version 410

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;

layout(location = 0) out vec2 fragUv;

uniform mat4 viewProjMatrix;
uniform mat4 worldMat;

void main()
{
	fragUv = vertexUv;
	gl_Position = viewProjMatrix * vec4((vec4(vertexPos, 1.0f) * worldMat).xyz, 1.0f);
}