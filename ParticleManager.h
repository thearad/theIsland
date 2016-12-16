#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <map>

#include "Window.h"
#include "Camera.h"
#include "util.h"
#include "Particle.h"

#define MAX_PARTICLES 1500
#define PARTICLE_PATH "../particles/"

using namespace util;

struct ParticleTexture {
	Texture* tex;
	GLuint texId;
	GLuint numRows;
};

class ParticleManager {
private:
	const float VERTICES[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	GLuint VAO, VBO_vert, VBO_models;
	GLuint shaderProgram;
	std::vector<Particle> particles;
	std::vector<glm::mat4> models;
	
	ParticleTexture texture;

public:
	ParticleManager(GLuint shaderProgram);
	~ParticleManager();
	void init();
	void bindData();

	void loadParticleModel(glm::vec3 pos, float rotation, float scale, glm::mat4 viewMat);
	void pre_render();
	void render(Camera camera);
	void post_render();

	void update();
	void addParticle(int x, int y);
	void generate(float delta, int x, int z);
};