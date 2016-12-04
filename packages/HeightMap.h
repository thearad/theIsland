#pragma once
#include "PerlinNoise.h"
#include "util.h"
#include "Window.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include <vector>
#include <cstdlib>
#include <time.h> 

using namespace util;
class HeightMap {
private:
	GLuint VAO, VBO_vert, VBO_norm, VBO_tex, EBO;
	GLfloat ISLAND_SIZE = 20.f;
	std::vector < glm::vec3 > vertices;
	std::vector < GLuint > indices;
	std::vector < glm::vec3 > normals;
	std::vector < glm::vec2 > texCoords;

	std::vector <std::pair<Texture, std::string>> textures;

	int width, height;

	void calcNormals();
	void calcIndices();
	void calcTexCoords();

	void loadTextures();
	void init();

	void loadVertices(char* filename, GLfloat island_size); //will load a PPM file
	void genVertices(int x, int z, GLfloat island_size);

	void bindData();

public:
	HeightMap(char* filename, GLfloat island_size = 20.f);
	HeightMap(int width, int height, GLfloat island_size = 20.f);

	void draw(GLuint shaderProgram);
	void drawNormals(GLuint shaderProgram);
	void quickDraw();

	void refresh(int width = 0, int height = 0, GLfloat island_size = 0.f);
	void refresh(char* filename, GLfloat island_size = 0.f);

	void setIslandSize(GLfloat radius);
};