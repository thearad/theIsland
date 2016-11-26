#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <fstream>
#include <iostream>

#define RGB_COMPONENT_COLOR 255

namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 getTrackballCoordinates(int width, int height, glm::vec2 point);
	unsigned char* loadPPM(char* filename, int &width, int &height);
}
