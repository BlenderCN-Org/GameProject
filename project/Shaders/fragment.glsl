#version 410

layout(location=0) in vec2 fragUv;

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(fragUv.x, fragUv.y, 0, 1);
}