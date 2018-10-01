#version 410

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 fragmentColor;

layout(location = 1) out vec4 red;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	fragmentColor = vec4(textColor, 1.0) * sampled;
	red = vec4(1.0);
}