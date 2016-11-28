#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec4 clippingPlane;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    gl_ClipDistance[0]=dot(model* vec4(position, 1.f), clippingPlane);

    gl_Position = projection * view * model * vec4(position, 1.f);
    
    //http://stackoverflow.com/questions/14196252/rotate-normals-in-shader
    mat3 NormalMatrix = transpose(inverse(mat3(model)));    
    Normal = normalize(NormalMatrix * normal);
        
    FragPos = vec3(model * vec4(position, 1.0));
    
    TexCoord = texCoord;
}