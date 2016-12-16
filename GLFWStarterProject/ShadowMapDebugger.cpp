#include "ShadowMapDebugger.h"
ShadowMapDebugger::ShadowMapDebugger() {
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void ShadowMapDebugger::draw(GLuint shaderProgram, GLuint depth_texture) {
	glDisable(GL_COMPARE_R_TO_TEXTURE);

	// Bind our texture in Texture Unit 8
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, depth_texture);

	// Set our "renderedTexture" sampler to user Texture Unit 8
	glUniform1i(glGetUniformLocation(shaderProgram, "texture_"), 8);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	glDisableVertexAttribArray(0);
	glEnable(GL_COMPARE_R_TO_TEXTURE);
}
