#include "ParticleManager.h"

ParticleManager::ParticleManager(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;
	init();

	std::vector<std::pair<GLenum, GLint>> texParams = {
		std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR),
		std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
	};

	texture.texId = Texture("../particles/firefly_green.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, SOIL_LOAD_RGBA, texParams).getID();
	texture.numRows = 1;

}

ParticleManager::~ParticleManager() {
}

void ParticleManager::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vert);
	glGenBuffers(1, &VBO_models);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_models);
	glBufferData(GL_ARRAY_BUFFER, models.size()*sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleManager::bindData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_models);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleManager::pre_render() {
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
}

void ParticleManager::post_render() {
	glDepthMask(true);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void ParticleManager::loadParticleModel(glm::vec3 pos, float rotation, float scale, glm::mat4 viewMat) {
	//REMEMBER GLM IS IN COL-ROW NOT ROW-COL

	//move particle position by pos
	glm::mat4 modelMat= glm::translate(glm::mat4(1.f), pos);

	std::cout << "VIEWMAT: " << glm::to_string(viewMat) << std::endl;
	std::cout << "MODELMAT: " << glm::to_string(modelMat) << std::endl;

	//unproject model
	float* modelArr = mat4ToArr(modelMat);
	float* viewArr = mat4ToArr(viewMat);

	//https://youtu.be/6PkjU9LaDTQ?t=10m12s
	modelArr[m00] = viewArr[m00];
	modelArr[m01] = viewArr[m10];
	modelArr[m02] = viewArr[m20];
	modelArr[m10] = viewArr[m01];
	modelArr[m11] = viewArr[m11];
	modelArr[m12] = viewArr[m21];
	modelArr[m20] = viewArr[m02];
	modelArr[m21] = viewArr[m12];
	modelArr[m22] = viewArr[m22];
	//std::cout << "MODELARR \n{ " << std::endl;
	//for (int i = 0; i < 16; i++) {
	//	std::cout << modelArr[i] << ", ";
	//	if (i % 4 == 0) std::cout << std::endl;
	//}
	//std::cout <<"} " << std::endl;
	//copy data inside modelArr back into modelMat
	memcpy(glm::value_ptr(modelMat), modelArr, sizeof(modelArr)*sizeof(float));
	//std::cout << "MODELMAT AFTER: " << glm::to_string(glm::make_mat4(modelArr)) << std::endl;
	////apply rotations and scale
	modelMat = glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale)) * modelMat;

	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelMat[0][0]);
	models.push_back(modelMat);
	std::cout << "model is " << glm::to_string(modelMat) << std::endl;
}

void ParticleManager::render(Camera camera) {
	update();
	models.clear();
	glm::mat4 viewMat = camera.GetViewMatrix();
	for (int i = 0; i < particles.size(); i++) {
		loadParticleModel(particles[i].position, particles[i].rotation, particles[i].scale, viewMat);
	}
	bindData();

	pre_render();

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &viewMat[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.texId);
	
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());
	
	glBindTexture(GL_TEXTURE_2D, 0);

	post_render();
}

void ParticleManager::update() {
	std::cout << "Updating particle vector" << std::endl;
	auto it = particles.begin();

	while (it != particles.end()) {
		if (it->update()) {
			it++;
		}
		else {
			it = particles.erase(it);
		}
	}
}

void ParticleManager::addParticle(int x, int z) {

	//16:39
	float x_c = randFrom(0, x / 2);
	if (rand() % 2 < 1) {
		x_c = -x_c;
	}
	float z_c = randFrom(0, z);
	if (rand() % 2 < 1) {
		z_c = -z_c;
	}
	//std::cout << "Adding a particle x: " << x_c << " z: " << z_c << std::endl;

	/*
	TODO: weirdest bug...
	1. randFrom() -> particles.push_back(Particle(glm::vec3(), ...) does not work.
	2. randFrom() -> particles.push_back(Particle(glm::vec3(), ...) -> do something with
	with particles[particles.size()-1] does work.

	3.particles.push_back(...) works
	4. randFrom() -> glm::vec3 to_push() -> particles.push_back(to_push, ...) works.
	*/
	glm::vec3 to_push = glm::vec3(x_c, 0.f, z_c);
	particles.push_back(Particle(to_push,glm::vec3(3, 3, 3), 1, 10, 0, 0.3f));
	//std::cout << "particles size:" << particles.size() << std::endl;

}

void ParticleManager::generate(float delta, int x, int z) {
	int newparticles = (int)(delta*100.0);
	if (newparticles > (int)(0.016f*100.0))
		newparticles = (int)(0.016f*100.0);

	for (int i = 0; i<newparticles; i++) {
		addParticle(x, z);
	}
}