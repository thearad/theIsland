#pragma once

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <vector>

class ShadowMapDebugger {
private:
	const GLfloat g_quad_vertex_buffer_data[18] = {
		-640.0f, -480.0f, 0.0f,
		640.0f, -480.0f, 0.0f,
		-640.0f,  480.0f, 0.0f,
		-640.0f,  480.0f, 0.0f,
		640.0f, -480.0f, 0.0f,
		640.0f,  480.0f, 0.0f,
	};
	GLuint quad_vertexbuffer;
public:
	ShadowMapDebugger();
	~ShadowMapDebugger();

	void draw(GLuint shaderProgram, GLuint depth_texture);

};