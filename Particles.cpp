//#include "Particles.h"
//
//Particles::Particles(GLuint shaderProgram) {
//	this->shaderProgram = shaderProgram;
//	// Vertex shader
//	cam_right_ID = glGetUniformLocation(shaderProgram, "cam_right");
//	cam_up_ID = glGetUniformLocation(shaderProgram, "cam_up");
//	view_ID = glGetUniformLocation(shaderProgram, "projection");
//	proj_ID = glGetUniformLocation(shaderProgram, "view");
//	//tex_ID = glGetUniformLocation(shaderProgram, "tex");
//
//	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
//	g_particule_color_data = new GLubyte[MaxParticles * 4];
//	for (int i = 0; i<MaxParticles; i++) {
//		ParticlesContainer[i].life = -1.0f;
//		ParticlesContainer[i].cameradistance = -1.0f;
//	}
//
//	init();
//}
//
//void Particles::init() {
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glGenBuffers(1, &billboard_vertex_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//	// The VBO containing the positions and sizes of the particles
//	glGenBuffers(1, &particles_position_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
//	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
//	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
//
//	// The VBO containing the colors of the particles
//	glGenBuffers(1, &particles_color_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
//	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
//	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}
//
//bool first = true;
//double lastTime;
//void Particles::time() {
//	if (first) {
//		delta = 0.0;
//		lastTime = glfwGetTime();
//		first = false;
//	}
//	else {
//		double currentTime = glfwGetTime();
//		delta = currentTime - lastTime;
//		lastTime = currentTime;
//	}
//}
//
//void Particles::addParticles() {
//	// Generate 10 new particule each millisecond,
//	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
//	// newparticles will be huge and the next frame even longer.
//	int newparticles = (int)(delta*10000.0);
//	if (newparticles > (int)(0.016f*10000.0))
//		newparticles = (int)(0.016f*10000.0);
//
//	for (int i = 0; i<newparticles; i++) {
//		int particleIndex = FindUnusedParticle();
//		ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
//		ParticlesContainer[particleIndex].pos = glm::vec3(0, 5, 5);
//
//		float spread = 1.5f;
//		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
//		// Very bad way to generate a random direction; 
//		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
//		// combined with some user-controlled parameters (main direction, spread, etc)
//		glm::vec3 randomdir = glm::vec3(
//			(rand() % 2000 - 1000.0f) / 1000.0f,
//			(rand() % 2000 - 1000.0f) / 1000.0f,
//			(rand() % 2000 - 1000.0f) / 1000.0f
//		);
//
//		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
//
//
//		// Very bad way to generate a random color
//		ParticlesContainer[particleIndex].r = rand() % 256;
//		ParticlesContainer[particleIndex].g = rand() % 256;
//		ParticlesContainer[particleIndex].b = rand() % 256;
//		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;
//
//		ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
//		std::cout << "particle[" << particleIndex << "]: life=" << ParticlesContainer[particleIndex].life << ", pos=" << 
//			glm::to_string(ParticlesContainer[particleIndex].pos) << std::endl;
//	}
//}
//
//void Particles::update(Camera camera) {
//	// Simulate all particles
//	ParticlesCount = 0;
//	for (int i = 0; i<MaxParticles; i++) {
//
//		Particle& p = ParticlesContainer[i]; // shortcut
//
//		if (p.life > 0.0f) {
//
//			// Decrease life
//			p.life -= delta;
//			if (p.life > 0.0f) {
//
//				// Simulate simple physics : gravity only, no collisions
//				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
//				p.pos += p.speed * (float)delta;
//				p.cameradistance = glm::length2(p.pos - camera.Position);
//				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;
//
//				// Fill the GPU buffer
//				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
//				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
//				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;
//
//				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;
//				std::cout << "g_part[" << 4 * ParticlesCount << "]: pos=" << glm::to_string(p.pos) << ", size=" <<
//					p.size << std::endl;
//
//				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
//				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
//				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
//				g_particule_color_data[4 * ParticlesCount + 3] = p.a;
//
//			}
//			else {
//				// Particles that just died will be put at the end of the buffer in SortParticles();
//				p.cameradistance = -1.0f;
//			}
//
//			ParticlesCount++;
//
//		}
//	}
//	SortParticles();
//}
//
//void Particles::bindData() {
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
//	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
//	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);
//
//	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
//	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
//	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);	
//	glBindVertexArray(0);
//}
//
//void Particles::render(Camera camera) {
//	time();
//	std::cout << "delta is " << delta << std::endl;
//	addParticles();
//	update(camera);
//	bindData();
//
//	glBindVertexArray(VAO);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glUniform3f(cam_right_ID, camera.Right.x, camera.Right.y, camera.Right.z);
//	glUniform3f(cam_up_ID, camera.Up.x, camera.Up.y, camera.Up.z);
//	glUniformMatrix4fv(proj_ID, 1, GL_FALSE, &Window::P[0][0]);
//	glUniformMatrix4fv(view_ID, 1, GL_FALSE, &Window::V[0][0]);
//
//	// 1rst attribute buffer : vertices
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
//	glVertexAttribPointer(
//		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//	);
//
//	// 2nd attribute buffer : positions of particles' centers
//	glEnableVertexAttribArray(1);
//	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
//	glVertexAttribPointer(
//		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//		4,                                // size : x + y + z + size => 4
//		GL_FLOAT,                         // type
//		GL_FALSE,                         // normalized?
//		0,                                // stride
//		(void*)0                          // array buffer offset
//	);
//
//	// 3rd attribute buffer : particles' colors
//	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
//	glVertexAttribPointer(
//		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//		4,                                // size : r + g + b + a => 4
//		GL_UNSIGNED_BYTE,                 // type
//		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
//		0,                                // stride
//		(void*)0                          // array buffer offset
//	);
//
//	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
//	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
//	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1
//
//								 // Draw the particules !
//								 // This draws many times a small triangle_strip (which looks like a quad).
//								 // This is equivalent to :
//								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
//								 // but faster.
//	std::cout << "ParticlesCount is " << ParticlesCount << std::endl;
//	GLenum err;
//	if ((err = glGetError()) != GL_NO_ERROR) {
//		std::cerr << "OpenGL error: " << err << std::endl;
//	}
//	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);
//
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(2);
//
//	glDisable(GL_BLEND);
//
//	glBindVertexArray(0);
//}
//
//// Finds a Particle in ParticlesContainer which isn't used yet.
//// (i.e. life < 0);
//int Particles::FindUnusedParticle() {
//
//	for (int i = LastUsedParticle; i<MaxParticles; i++) {
//		if (ParticlesContainer[i].life <= 0) {
//			LastUsedParticle = i;
//			return i;
//		}
//	}
//
//	for (int i = 0; i<LastUsedParticle; i++) {
//		if (ParticlesContainer[i].life <= 0) {
//			LastUsedParticle = i;
//			return i;
//		}
//	}
//
//	return 0; // All particles are taken, override the first one
//}
//
//void Particles::SortParticles() {
//	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
//}