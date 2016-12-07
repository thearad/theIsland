#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include "util.h"

using namespace util;

struct ParticleTexture {
	GLuint texId;
	GLuint numRows;
};

class Particle{
private:

public:
	glm::vec3 position;
	glm::vec3 velocity;

	float gravity_effect;
	float life_length;
	float rotation;
	float scale;

	float prev_time;
	float curr_time;
	float elapsed_time;

	ParticleTexture texture;

	Particle(glm::vec3 position, glm::vec3 velocity, float gravity_effect,
		float life_length, float rotation, float scale, ParticleTexture texture);
	bool update();

};