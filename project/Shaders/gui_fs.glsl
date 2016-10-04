#version 430

layout(location = 0) in vec2 TexCoords;
out vec4 color;

uniform sampler2D guiTexture;

void main()
{    
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(guiTexture, TexCoords).r);
    //color = vec4(1.0)*sampled;
    color = vec4(TexCoords, 1.0, 1.0);
}  