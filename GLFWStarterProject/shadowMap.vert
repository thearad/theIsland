#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform vec3 lightInvDir;
uniform mat4 staticview;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 depthBiasMVP;
uniform vec4 clippingPlane;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 normal_cam;
out vec3 eyeDir;
out vec3 lightDir;
out vec4 shadowCoord;
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
    
	shadowCoord = depthBiasMVP * vec4(position,1.0);
    
	lightDir = (staticview * vec4(lightInvDir, 0.0)).xyz;//lightInvDir is freaking out

	eyeDir = vec3(0,0,0) - (staticview * model * vec4(position, 1.0)).xyz;
	normal_cam = (staticview * model * vec4(normal, 0.0)).xyz; ///took out view

	float distance = length(camSpace.xyz);
    Visibility = exp(-pow(distance*density, gradient));
    Visibility = clamp(Visibility, 0.0, 1.0);
}