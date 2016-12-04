#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec4 clippingPlane;

out vec3 TexCoords;

void main()
{
    gl_ClipDistance[0]=dot(model* vec4(position, 1.f), clippingPlane);

    gl_Position = projection * view * model * vec4(position, 1.0);  
    TexCoords = position;
}
