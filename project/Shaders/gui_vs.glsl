#version 430

layout(location = 0) in vec4 v;

in int gl_InstanceID;

layout(location = 0) out int instanceId;

void main()
{
    instanceId = gl_InstanceID;
    gl_Position = v;
}