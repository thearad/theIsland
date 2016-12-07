#version 330 core

layout(location = 0) in vec3 position;
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texCoords;

void main(void){

	//gl_Position = projection * view * model* vec4(position, 1.0);
	gl_Position = projection * view * instanceMatrix* vec4(position, 1.0);
	texCoords.x = position.x + 0.5;
	texCoords.y = 1.0 - (position.y + 0.5);
}