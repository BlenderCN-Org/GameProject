#version 410

layout(location = 0) in vec2 fragUv;

layout(location = 0) out vec4 fragmentColor;

//layout(location = 1) out vec4 red;
uniform sampler2D textureIN;
uniform vec3 selectedColor;
void main()
{
	vec4 color = vec4(selectedColor, 1.0);// texture(textureIN, vec2(fragUv.x, fragUv.y));
	fragmentColor = color; //vec4(fragUv.x, fragUv.y, 0, 1);
//	red = vec4(1.0, 0.0, 0, 1);
}