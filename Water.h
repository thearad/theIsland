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

	int width, height;

	//vertices to create a quad
	std::vector<glm::vec3> vertices = {
		glm::vec3( 1.0f, SEA_LEVEL,  1.0f ),
		glm::vec3(-1.0f, SEA_LEVEL,  1.0f),
		glm::vec3(-1.0f, SEA_LEVEL,  -1.0f),
		glm::vec3(1.0f, SEA_LEVEL,  -1.0f)
	};

	std::vector<GLuint> indices = {
		 0, 1, 2 ,
		 2, 3, 0
	};

	//Frame Buffers

public:
	GLuint reflectionFBO, reflectionTex, reflectionDBO;
	GLuint refractionFBO, refractionTex, refractionDTex;
	GLuint dudv;

	GLfloat WAVE_SPEED = 0.03f;
	float moveFactor = 0.f;

	bool first_draw = true;
	double lastTime;

	enum{REFLECTION=0, REFRACTION};
	Water(int width, int height);
	~Water();

	void init();
	void initFrameBuffers();

	void bindFrameBuffer(int type);
	void unbindFrameBuffer();
	
	void setDrawData(glm::vec3 camera_pos);
	void draw(GLuint shaderProgram);

	void genTexCoords();
};
