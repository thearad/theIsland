#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

#define RGB_COMPONENT_COLOR 255
#define PI 3.14159265358979323846

namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 getTrackballCoordinates(int width, int height, glm::vec2 point);
	unsigned char* loadPPM(char* filename, int &width, int &height);
	float* mat4ToArr(glm::mat4 mat);
	float degToRad(float);
	float randRad();
	float randFrom(int min, int max);

	//Corresponds to index of a Col-major 4x4 matrix such that matrix looks like
	//[a00, a10, a20, a30]
	//[a01, a11, a21, a31]
	//[a02, a12, a22, a33]
	//[a03, a13, a23, a33]
	enum MAT {
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33,
	};
}
