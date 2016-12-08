#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

class Sphere
{
public:
	Sphere();
	~Sphere();

	void draw(glm::mat4 M, GLuint shaderProgram);
	GLuint VBO, SAO, NBO;
	int tesselate, bezNumb;
	float time;
	GLint uProjection, uModelview;
	//glm::mat4 world;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereNormals;
	glm::vec3 start, tan1, tan2, end, curPoint;
};