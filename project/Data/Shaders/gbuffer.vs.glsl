#version 410

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec4 vertexColor;

layout(location = 0) out vec2 fragUv;
layout(location = 1) out vec3 pos;
layout(location = 2) out vec4 color;

uniform mat4 viewProjMatrix;
uniform mat4 worldMat;
uniform mat4 reflectMat;

void main()
{
	fragUv = vertexUv;
	pos = (vec4(vertexPos, 1.0) * worldMat).xyz;
	vec4 p2 = vec4(pos, 1.0) * reflectMat;
	gl_Position = viewProjMatrix * p2;
	color = vertexColor;
	
}