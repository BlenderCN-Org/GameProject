#version 410

layout(location = 1) in vec3 pos;
layout(location = 2) in vec3 norm;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 worldPos;

void main()
{
	fragmentColor = vec4(0.0);
	normalColor = vec4(1.0);
	worldPos = vec4(pos, 1.0);
}
