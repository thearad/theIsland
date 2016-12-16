#include "Particle.h"
Particle::Particle(
	glm::vec3 position, glm::vec3 velocity, float gravity_effect,
	float life_length, float rotation, float scale) {
	
	this->position = position;
	this->velocity = velocity;
	this->gravity_effect = gravity_effect;
	this->life_length = life_length;
	this->rotation = rotation;
	this->scale = scale;

	this->prev_time = (float)glfwGetTime();
}

/*
Updates position of particle based on velocity. 
Returns true if particle should still be alive. 
Returns false is particle is dead now 
*/
bool Particle::update() {
	curr_time = (float)glfwGetTime();
	float frame_time = curr_time - prev_time;
	if (rand() % 80 < 1) {
		velocity = glm::vec3(glm::rotate(glm::mat4(1.f), randRad(), glm::vec3(1.f, 0.f, 0.f)) * glm::vec4(velocity, 1.f));
		velocity = glm::vec3(glm::rotate(glm::mat4(1.f), randRad(), glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(velocity, 1.f));
		velocity = glm::vec3(glm::rotate(glm::mat4(1.f), randRad(), glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(velocity, 1.f));
	}
	prev_time = curr_time;

	glm::vec3 change = glm::vec3(velocity);
	change = glm::vec3(glm::scale(glm::mat4(1.f), glm::vec3(frame_time, frame_time, frame_time)) * glm::vec4(change, 1.f));

	position += change;
	if (position.y < 0.f)
		return false;
	elapsed_time += frame_time;

	return elapsed_time < life_length;

}