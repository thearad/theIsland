#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 clipSpace;

void main()
{
    clipSpace = projection * view * model * vec4(position.x, position.y, position.z, 1.f);;
    gl_Position = clipSpace;
    
    //http://stackoverflow.com/questions/14196252/rotate-normals-in-shader
    mat3 NormalMatrix = transpose(inverse(mat3(model)));    
    Normal = normalize(NormalMatrix * normal);
        
    FragPos = vec3(model * vec4(position, 1.0));
    
    TexCoords = vec2(position.x, position.y);
}