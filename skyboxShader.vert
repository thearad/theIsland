#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 modelview;

out vec3 TexCoords;

void main()
{
    gl_Position =   projection * modelview * vec4(position, 1.0);  
    TexCoords = position;
}
