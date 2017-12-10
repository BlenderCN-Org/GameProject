#version 410

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;
layout(location = 5) in ivec4 vertexColor;

//layout(location = 4) in vec4 vertexWeights;
//layout(location = 5) in vec4 vertexIndex;


layout(location = 0) out vec2 fragUv;
layout(location = 1) out vec3 pos;
layout(location = 2) out vec4 color;

uniform mat4 viewProjMatrix;
uniform mat4 worldMat;
uniform mat4 reflectMat;

uniform vec4 clipPlane;

vec4 plane = vec4(0.0, -1.0, 0.0, 2.0);

void main()
{
	fragUv = vertexUv;
	vec4 p = (vec4(vertexPos, 1.0) * worldMat);
	pos = p.xyz;
	vec4 p2 = vec4(pos, 1.0) * reflectMat;
	
	
	gl_ClipDistance[0] = dot(clipPlane, p);
	
	gl_Position = viewProjMatrix * p2;
	color = vertexColor;
	
}