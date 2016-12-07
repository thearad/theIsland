#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

uniform mat4 projection;
uniform mat4 view;

uniform vec3 cam_right;
uniform vec3 cam_up;

void main()
{
	float particle_size = xyzs.w;
	
	vec3 particle_pos = xyzs.xyz;

	vec3 vertex_pos = 
		particle_pos
		+ cam_right * squareVertices.x * 20.f;
		+ cam_up * squareVertices.y * 20.f;
    
	gl_Position = projection * view * vec4(vertex_pos, 1.0f);
}