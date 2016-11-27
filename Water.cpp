#include "Water.h"

Water::Water(int width, int height) {
	int l = floor(-width/2), r = floor(width / 2), u = floor(-height / 2), d = floor(height / 2);
	vertices.clear();
	vertices.push_back(glm::vec3(r, SEA_LEVEL, u));
	vertices.push_back(glm::vec3(l, SEA_LEVEL, u));
	vertices.push_back(glm::vec3(l, SEA_LEVEL, d));
	vertices.push_back(glm::vec3(r, SEA_LEVEL, d));

	init();
};

void Water::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Water::initFrameBuffers() {	
	//Create Frame Buffer for reflections
	glGenFramebuffers(1, &this->reflectionFBO);

	//Bind the newly generated frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->reflectionFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	//create reflection texture attachment for frame buffer
	Texture reflTexture = Texture(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	this->reflectionTex = reflTexture.getID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->reflectionTex, 0);

	//create depth buffer
	glGenRenderbuffers(1, &this->reflectionDBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->reflectionDBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->reflectionDBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Create Frame Buffer for refractions
	glGenFramebuffers(1, &this->refractionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->refractionFBO);
	
	//create refraction texture attachment and refraction depth texture attachment
	Texture refractionTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	this->refractionTex = refractionTexture.getID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->refractionTex, 0);

	Texture refractionDepthTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
	this->refractionDTex = refractionDepthTexture.getID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->refractionDTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::draw(GLuint shaderProgram) {
	glm::mat4 model = glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}