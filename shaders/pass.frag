#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D texture_;

in vec2 UV;

void main(){
	color = texture(texture_, UV);
	//color = (1,1,1,1);
}
