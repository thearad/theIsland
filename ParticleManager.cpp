#include "ParticleManager.h"

ParticleManager::ParticleManager(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;

	std::vector<std::pair<GLenum, GLint>> texParams = {
		std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR),
		std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
	};

	texture = ParticleTexture(Texture("../particles/star.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, texParams).getID(), 1);

	init();
}

ParticleManager::~ParticleManager() {
}

void ParticleManager::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vert);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void ParticleManager::pre_render() {
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
}

void ParticleManager::post_render() {
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
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
	//modelMat = glm::rotate(glm::mat4(1.f), degToRad(rotation), glm::vec3(0.f, 0.f, 1.f)) * modelMat;
	//modelMat = glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, scale)) * modelMat;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelMat[0][0]);
}

void ParticleManager::render(Camera camera) {
	std::cout << "enter render" << std::endl;
	update();

	pre_render();

	glm::mat4 viewMat = camera.GetViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &viewMat[0][0]);

	//for (int i = 0; i < particles.size(); i++) {
	//	std::cout << "Drawing particles[" << i << "]" << std::endl;
	//	loadParticleModel(particles[i].position, particles[i].rotation, particles[i].scale, viewMat);
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
	//}
	for (auto map_it = particles_map.begin(); map_it != particles_map.end(); map_it++) {
		std::vector<Particle*>* l = map_it->second;
		ParticleTexture tex = map_it->first;

		std::cout << "map[" << map_it->first.texId << "]=";
		//Grab ParticleList for each ParticleTexture
		for (int i = 0; i < l->size(); i++) {
			Particle* part = l->at(i);
			std::cout << glm::to_string(part->position) << ", ";
		}
		std::cout << std::endl;

		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.texId);
		for (auto vec_it = l->begin(); vec_it != l->end(); vec_it++) {
			std::cout << "Drawing particles[_]" << std::endl;
			Particle* part = *vec_it;
			loadParticleModel(part->position, part->rotation, part->scale, viewMat);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);
		}
		//6:30 shader work.
	}

	post_render();
}

void ParticleManager::update() {
	std::cout << "Updating particle vector" << std::endl;

	//Iterate through map of ParticleTexture, ParticleList
	auto map_it = particles_map.begin();
	while (map_it != particles_map.end()) {
		std::vector<Particle*>* l = map_it->second;

		//Iterate through ParticleList
		auto vec_it = l->begin();
		while (vec_it != l->end()) {
			//If Particle in ParticleList is updated, go to next item

			Particle* part = *vec_it;
			if (part->update()) {
				vec_it++;
			}

			//Else erase the Particle from ParticleList. If ParticleList is empty after this,
			//Erase the ParticleList from the map.
			else {
				vec_it = l->erase(vec_it);
				if (l->empty()) {
					particles_map.erase(map_it);
				}
			}
		}
	}


	//auto it = particles.begin();

	//while (it != particles.end()) {
	//	if (it->update()) {
	//		it++;
	//	}
	//	else {
	//		it = particles.erase(it);
	//	}
	//}
}

void ParticleManager::addParticles(int x, int z) {

	//16:39
	float x_c = randFrom(0, x / 2);
	if (rand() % 2 < 1) {
		x_c = -x_c;
	}
	float z_c = randFrom(0, z);
	std::cout << "Adding a particle x: " << x_c << " z: " << z_c << std::endl;

	/*
	TODO: weirdest bug...
	1. randFrom() -> particles.push_back(Particle(glm::vec3(), ...) does not work.
	2. randFrom() -> particles.push_back(Particle(glm::vec3(), ...) -> do something with
	with particles[particles.size()-1] does work.

	3.particles.push_back(...) works
	4. randFrom() -> glm::vec3 to_push() -> particles.push_back(to_push, ...) works.
	*/
	glm::vec3 to_push = glm::vec3(x_c, 0.f, z_c);
	//particles.push_back(Particle(to_push,glm::vec3(3, 3, 3),1, 5, 0, 1, texture));
	std::cout << "particles size:" << particles.size() << std::endl;

	std::vector<Particle*>* l = particles_map[texture];
	if (l == NULL) {
		std::cout << "l was null" << std::endl;
		particles_map[texture] = new std::vector<Particle*>{new Particle(to_push, glm::vec3(3, 3, 3), 1, 5, 0, 1, texture)};
	}
	//particles.push_back(Particle(to_push, glm::vec3(3, 3, 3), 1, 5, 0, 1, texture));

}