#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Texture.h"

#define SEA_LEVEL 0.01f
#define REFLECTION_WIDTH 320
#define REFLECTION_HEIGHT 180
#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 720

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

	//Frame Buffers
	GLuint reflectionFBO, reflectionTex, reflectionDBO;
	GLuint refractionFBO, refractionTex, refractionDTex;
public:
	Water(int width, int height);
	~Water();

	void init();
	void initFrameBuffers();
	GLuint creatDepthBuffer(int width, int height);
	void draw(GLuint shaderProgram);
};
