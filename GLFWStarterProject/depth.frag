#version 330 core

// Ouput data
layout(location = 0) out float fragmentdepth;
//out vec4 color;

void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
	//color = vec4(1,1,0,1);
}