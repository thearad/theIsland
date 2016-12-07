//#pragma once
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "glm/gtx/string_cast.hpp"
//#include <vector>
//
//#include "Window.h"
//#include "Camera.h"
//#include "util.h"
//
//// CPU representation of a particle
//struct Particle {
//	glm::vec3 pos, speed;
//	unsigned char r, g, b, a; // Color
//	float size, angle, weight;
//	float life; // Remaining life of the particle. if <0 : dead and unused.
//	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
//
//	bool operator<(const Particle& that) const {
//		// Sort in reverse order : far particles drawn first.
//		return this->cameradistance > that.cameradistance;
//	}
//};
//
//
//const int MaxParticles = 100000;
//
//class Particles {
//private:
//	Particle ParticlesContainer[MaxParticles];
//	int LastUsedParticle = 0;
//
//	const float g_vertex_buffer_data[12] = {
//		-0.5f, -0.5f, 0.0f,
//		0.5f, -0.5f, 0.0f,
//		-0.5f, 0.5f, 0.0f,
//		0.5f, 0.5f, 0.0f
//	};
//
//	GLuint cam_right_ID, cam_up_ID, view_ID, proj_ID, tex_ID;
//	
//	GLuint VAO;
//	GLuint billboard_vertex_buffer;
//	GLuint particles_position_buffer;
//	GLuint particles_color_buffer;
//
//	GLuint shaderProgram;
//
//	int ParticlesCount;
//
//	GLfloat* g_particule_position_size_data;
//	GLubyte* g_particule_color_data;
//
//	double delta = 0.0;
//public:
//	Particles(GLuint shaderProgram);
//	int FindUnusedParticle();
//	void SortParticles();
//
//	void init();
//
//	void time();
//	void addParticles();
//	void update(Camera camera);
//	void bindData();
//	void render(Camera camera);
//
//};