#version 410

layout(location = 0) in vec2 fragUv;
layout(location = 1) in vec3 pos;

layout(location = 0) out vec4 fragmentColor;

uniform vec3 selectedColor;

void main()
{
	fragmentColor = vec4(selectedColor, 1.0);
}
