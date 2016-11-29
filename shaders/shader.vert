#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 camera_Position;

uniform int quad_width;
uniform int quad_height;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 clipSpace;
out float Visibility;
out vec3 toCameraVec;

const float tiling = 6.0;
const float density = 0.017;
const float gradient = 5.5;
void main()
{
    vec4 worldSpace = model * vec4(position, 1.f);
    vec4 camSpace = view * worldSpace;
    clipSpace = projection * camSpace;;
    gl_Position = clipSpace;
    
    
    //http://stackoverflow.com/questions/14196252/rotate-normals-in-shader
    mat3 NormalMatrix = transpose(inverse(mat3(model)));    
    Normal = normalize(NormalMatrix * normal);
        
    FragPos = vec3(model * vec4(position, 1.0));
    
    TexCoords = vec2(position.x/quad_width, position.z/quad_height)*tiling;
    
    float distance = length(camSpace.xyz);
    Visibility = exp(-pow(distance*density, gradient));
    Visibility = clamp(Visibility, 0.0, 1.0);
    
    toCameraVec = camera_Position - worldSpace.xyz;
}