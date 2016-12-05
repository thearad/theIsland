#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 mvp; // Model View Projection Matrix

uniform float length = 1.0;

in vec3 vertex_normal[];

out vec3 vertex_color;

void main(){
	vec3 normal = vertex_normal[0];
	
	vertex_color = abs(normal);
	
	vec4 v0 = gl_in[0].gl_Position;
	gl_Position = mvp * v0;
	EmitVertex();
	
	vec4 v1 = v0 + vec4(normal * length, 0.0);
	gl_Position = mvp * v1;
	EmitVertex();
	
	EndPrimitive();
}