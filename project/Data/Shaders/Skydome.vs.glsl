#version 410

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;

layout(location = 0) out vec2 fragUv;
layout(location = 1) out vec3 pos;
layout(location = 2) out vec3 norm;

uniform mat4 viewProjMatrix;
uniform mat4 worldMat;

vec4 plane = vec4(0.0, 0.0, 0.0, 0.0);

void main()
{
	fragUv = vertexUv;
	pos = (vec4(vertexPos, 1.0) * worldMat).xyz;
	gl_Position = viewProjMatrix * vec4(pos, 1.0);
	gl_ClipDistance[0] = dot(plane, gl_Position);
	norm = normalize(-vertexPos);
	
}