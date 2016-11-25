#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
}
