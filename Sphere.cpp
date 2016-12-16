#include "Sphere.h"
#include "Window.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere()
{
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &SAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	glDisable(GL_CULL_FACE);

	glBindVertexArray(SAO);

	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW); // Don't give it any data
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind normals to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW); // Don't give it any data
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Sphere::~Sphere()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &SAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
}

void Sphere::draw(glm::mat4 M, GLuint shaderProgram)
{
	float radius = 3;
	int stacks = 20;
	int slices = 20;
	float prevStacks = 0, prevSlices = 0;
	if (prevStacks != stacks || prevSlices != slices) {
		prevStacks = stacks; prevSlices = slices;
		sphereVertices.clear();
		sphereNormals.clear();
		float fstacks = (float)stacks;
		float fslices = (float)slices;
		for (int i = 0; i < slices; i++) {
			for (int j = 0; j < stacks; j++) {
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices),
					radius * -cos(M_PI * (j + 1.0f) / fslices),
					radius * sin(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices),
					-cos(M_PI * (j + 1.0f) / fslices),
					sin(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices))));
				// Top right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * (j + 1.0) / fslices),
					radius * -cos(M_PI * (j + 1.0) / fslices),
					radius * sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * (j + 1.0) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * (j + 1.0) / fslices),
					-cos(M_PI * (j + 1.0) / fslices),
					sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * (j + 1.0) / fslices))));
				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices),
					radius * -cos(M_PI * j / fslices),
					radius * sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices),
					-cos(M_PI * j / fslices),
					sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices))));

				// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
				// of array indices.
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices),
					radius * -cos(M_PI * (j + 1.0f) / fslices),
					radius * sin(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices),
					-cos(M_PI * (j + 1.0f) / fslices),
					sin(2.0f * M_PI * i / fstacks) * sin(M_PI * (j + 1.0f) / fslices))));
				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices),
					radius * -cos(M_PI * j / fslices),
					radius * sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices),
					-cos(M_PI * j / fslices),
					sin(2.0f * M_PI * (i + 1.0) / fstacks) * sin(M_PI * j / fslices))));
				// Bottom left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * M_PI * i / fstacks) * sin(M_PI * j / fslices),
					radius * -cos(M_PI * j / fslices),
					radius * sin(2.0f * M_PI * i / fstacks) * sin(M_PI * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * M_PI * i / fstacks) * sin(M_PI * j / fslices),
					-cos(M_PI * j / fslices),
					sin(2.0f * M_PI * i / fstacks) * sin(M_PI * j / fslices))));

			}
		}

		// Now bind this new vertex data
		glBindVertexArray(SAO);

		// Bind vertices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereVertices.size(), &sphereVertices[0], GL_STATIC_DRAW);

		// Bind normals
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereNormals.size(), &sphereNormals[0], GL_STATIC_DRAW);

		// Done updating the buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	//world = M;
	//M = glm::translate(M, Window::Maxpt);
	//printf("Maxpt..... <%f %f %f>", Window::Maxpt.x, Window::Maxpt.y, Window::Maxpt.z);
	glm::mat4 modelview = Window::V * M;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "model");
	GLint uView = glGetUniformLocation(shaderProgram, "view");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform1f(glGetUniformLocation(shaderProgram, "isSphere"), 1);
	// Draw the sphere regardless of whether it was previously updated or not
	glBindVertexArray(SAO);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());
	glBindVertexArray(0);
}