#include "util.h"
namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		return glm::normalize(glm::cross(U, V));
	}
}
