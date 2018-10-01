#version 410

layout(location = 0) in vec3 vertexPosition_modelspace;

layout(location = 0) out vec2 UV;

void main()
{
	UV = (vertexPosition_modelspace.xy + vec2(1.0)) * 0.5;
	gl_Position = vec4(vertexPosition_modelspace,1);
}