#include "Water.h"

Water::Water(int width, int height) {
	int l = floor(-width/2), r = floor(width / 2), u = floor(-height / 2), d = floor(height / 2);
	vertices.clear();
	vertices.push_back(glm::vec3(r, SEA_LEVEL, u));
	vertices.push_back(glm::vec3(l, SEA_LEVEL, u));
	vertices.push_back(glm::vec3(l, SEA_LEVEL, d));
	vertices.push_back(glm::vec3(r, SEA_LEVEL, d));

	init();
	initFrameBuffers();
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
	std::cout << "init Frame buffers" << std::endl;
	//REFLECTION
	glGenFramebuffers(1, &this->reflectionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->reflectionFBO);

	glGenTextures(1, &reflectionTex);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &reflectionDBO);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDBO);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTex, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer object creaion failed." << std::endl;
	//REFRACTION
	//glGenFramebuffers(1, &this->refractionFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, this->refractionFBO);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//glGenTextures(1, &refractionTex);
	//glBindTexture(GL_TEXTURE_2D, refractionTex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFRACTION_WIDTH, REFRACTION_HEIGHT,0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTex, 0);

	//glGenTextures(1, &refractionDTex);
	//glBindTexture(GL_TEXTURE_2D, refractionDTex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, refractionDTex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////Create Frame Buffer for reflections
	//glGenFramebuffers(1, &this->reflectionFBO);

	////Bind the newly generated frame buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, this->reflectionFBO);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//
	////create reflection texture attachment for frame buffer
	//Texture reflTexture = Texture(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	//this->reflectionTex = reflTexture.getID();
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->reflectionTex, 0);

	////create depth buffer
	//glGenRenderbuffers(1, &this->reflectionDBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, this->reflectionDBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->reflectionDBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	////Create Frame Buffer for refractions
	//glGenFramebuffers(1, &this->refractionFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, this->refractionFBO);
	//
	////create refraction texture attachment and refraction depth texture attachment
	//Texture refractionTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	//this->refractionTex = refractionTexture.getID();
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->refractionTex, 0);

	//Texture refractionDepthTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
	//this->refractionDTex = refractionDepthTexture.getID();
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->refractionDTex, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::draw(GLuint shaderProgram) {
	glm::mat4 model = glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Water::bindFrameBuffer(int type) {
	int w, h;
	GLuint buf;

	if (type == REFLECTION) {
		w = REFLECTION_WIDTH, h = REFLECTION_HEIGHT;
		buf = reflectionFBO;
	}
	else if (type == REFRACTION){
		w = REFRACTION_WIDTH, h = REFRACTION_HEIGHT;
		buf = refractionFBO;
	}
	else {
		std::cerr << "Water::bindFrameBuffer - invalid type" << std::endl;
		return;
	}
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
	glViewport(0, 0, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Water::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::width, Window::height);
}