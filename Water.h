#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

#define SEA_LEVEL 0.01f
class Water {
private:
	GLuint VAO, VBO, EBO;

	//vertices to create a quad
	std::vector<glm::vec3> vertices = {
		glm::vec3( 1.0f, SEA_LEVEL,  1.0f ),
		glm::vec3(-1.0f, SEA_LEVEL,  1.0f),
		glm::vec3(-1.0f, SEA_LEVEL,  -1.0f),
		glm::vec3(1.0f, SEA_LEVEL,  -1.0f)
	};

	GLuint indices[2][3] = {
		{ 0, 1, 2 },
		{ 2, 3, 0 }
	};
public:
	Water(int width, int height);
	~Water();

	void init();
	void draw(GLuint shaderProgram);
};
