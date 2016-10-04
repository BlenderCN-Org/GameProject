
#version 430
#extension GL_ARB_shader_storage_buffer_object : enable

layout(points) in;
layout(triangle_strip, max_vertices = 4) out; 

struct Quad
{
    vec4 pos0;  
    vec4 pos1;
    vec4 pos2;
    vec4 pos3;
};

layout(std430, binding = 1) buffer matriceBuffer
{
    Quad matrices[];
    //mat4 mats[];
};


layout(location = 0) in int instanceId[1];

layout(location = 0) out vec2 TexCoords;

void main()
{
    if(matrices.length() > instanceId[0])
    {
        Quad q = matrices[instanceId[0]];
        
        gl_Position = vec4(q.pos0.xy, 0, 1) + gl_in[0].gl_Position;
        
        EmitVertex();
        
        gl_Position = vec4(q.pos1.xy, 0, 1);
        
        EmitVertex();
        
        gl_Position = vec4(q.pos2.xy, 0, 1);
        
        EmitVertex();

        gl_Position = vec4(q.pos3.xy, 0, 1);
        
        EmitVertex();
        
        EndPrimitive();
    }

}