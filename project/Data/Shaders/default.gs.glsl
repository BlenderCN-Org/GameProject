#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec2 fragUv[];

layout(location = 0) out vec2 UV;

void main() {

UV = fragUv[0];
gl_Position = gl_in[0].gl_Position;
EmitVertex();

UV = fragUv[1];
gl_Position = gl_in[1].gl_Position;
EmitVertex();

UV = fragUv[2];
gl_Position = gl_in[2].gl_Position;
EmitVertex();
EndPrimitive();

}