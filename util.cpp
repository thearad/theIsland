#include "util.h"
namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		return glm::normalize(glm::cross(U, V));
	}

	glm::vec3 getTrackballCoordinates(int width, int height, glm::vec2 point) {
		glm::vec3 v;
		v.x = (2.0*point.x - width) / width;
		v.y = (height - 2.0*point.y) / height;
		v.z = 0;
		double d = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));;
		d = (d < 1.f) ? d : 1.f;
		v.z = sqrt(1.001 - d*d);

		v = glm::normalize(v);

		return v;
	}
}
