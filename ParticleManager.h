#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <unordered_map>

#include "Window.h"
#include "Camera.h"
#include "util.h"
#include "Particle.h"

#define MAX_PARTICLES 1500
using namespace util;

struct ParticleHash {
	size_t operator()(const ParticleTexture& t) const {
		return t.texId;
	}
};

class ParticleManager {
private:
	const float VERTICES[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	GLuint VAO, VBO_vert;
	GLuint shaderProgram;
	std::vector<Particle*> particles;
	
	std::unordered_map<ParticleTexture, std::vector<Particle*>*, ParticleHash> particles_map;
	ParticleTexture texture;

public:
	ParticleManager(GLuint shaderProgram);
	~ParticleManager();
	void init();

	void loadParticleModel(glm::vec3 pos, float rotation, float scale, glm::mat4 viewMat);
	void pre_render();
	void render(Camera camera);
	void post_render();

	void update();
	void addParticles(int x, int y);
};