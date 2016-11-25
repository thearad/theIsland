#pragma once
#include "PerlinNoise.h"
#include "util.h"
#include "Window.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include <vector>

using namespace util;
class HeightMap {
private:
	GLuint VAO, VBO_vert, VBO_norm, EBO;
	std::vector < glm::vec3 > vertices;
	std::vector < GLuint > indices;
	std::vector < glm::vec3 > normals;

	int width, height;

public:
	HeightMap();
	void genMap(int x, int z);
	void init();
	void draw(GLuint shaderProgram);
};