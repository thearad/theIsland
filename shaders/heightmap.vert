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
out float Visibility;

const float density = 0.017;
const float gradient = 5.5;

void main()
{
    vec4 camSpace = view * model * vec4(position, 1.f);
    gl_ClipDistance[0]=dot(model* vec4(position, 1.f), clippingPlane);

    gl_Position = projection * camSpace;
    
    //http://stackoverflow.com/questions/14196252/rotate-normals-in-shader
    mat3 NormalMatrix = transpose(inverse(mat3(model)));    
    Normal = normalize(NormalMatrix * normal);
        
    FragPos = vec3(model * vec4(position, 1.0));
    
    TexCoord = texCoord;
    
    float distance = length(camSpace.xyz);
    Visibility = exp(-pow(distance*density, gradient));
    Visibility = clamp(Visibility, 0.0, 1.0);
}