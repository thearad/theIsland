#version 330 core

in vec2 texCoords;

uniform sampler2D particleTexture;

out vec4 color;

void main(void){

	color = texture(particleTexture, texCoords);

}