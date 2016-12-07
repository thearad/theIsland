#include "Water.h"

Water::Water(int width, int height) {
	int l = floor(-width/2), r = floor(width / 2), u = floor(-height / 2), d = floor(height / 2);
	vertices.clear();
	indices.clear();

	//generate vertices
	for (int h = floor(-height / 2); h < floor(height / 2); h++) {
		for (int w = floor(-width / 2); w < floor(width / 2); w++) {
			vertices.push_back(glm::vec3(w, SEA_LEVEL, h));
		}
	}

	//generate indices
	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {
			indices.push_back(h*width + w);
			indices.push_back((h + 1)*width + w);
			indices.push_back((h + 1)*width + (w + 1));

			indices.push_back((h + 1)*width + (w + 1));
			indices.push_back(h*width + (w + 1));
			indices.push_back(h*width + w);
		}
	}
	
	//save variables
	this->width = width;
	this->height = height;

	//load wave texture
	dudv = Texture("../water/waterDUDV.png").getID();

	//initialize everything
	init();
	bindData();
	initFrameBuffers();
};

void Water::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Water::bindData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Water::initFrameBuffers() {
	//REFLECTIONS
	glGenFramebuffers(1, &this->reflectionFBO);
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

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Water::initFrameBuffers - Failed to create reflection frame buffer" << std::endl;

	//Create Frame Buffer for refractions
	glGenFramebuffers(1, &this->refractionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->refractionFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//create refraction texture attachment and refraction depth texture attachment
	Texture refractionTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	this->refractionTex = refractionTexture.getID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->refractionTex, 0);

	Texture refractionDepthTexture = Texture(REFRACTION_WIDTH, REFRACTION_HEIGHT, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
	this->refractionDTex = refractionDepthTexture.getID();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->refractionDTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Water::initFrameBuffers - Failed to create refraction frame buffer with enum "<< glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::bindFrameBuffer(int type) {
	int w, h;
	GLuint buf;

	if (type == REFLECTION) {
		w = REFLECTION_WIDTH, h = REFLECTION_HEIGHT;
		buf = reflectionFBO;
	}
	else if (type == REFRACTION) {
		w = REFRACTION_WIDTH, h = REFRACTION_HEIGHT;
		buf = refractionFBO;
	}
	else {
		std::cerr << "Water::bindFrameBuffer - invalid type" << std::endl;
		return;
	}
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, buf);
	glViewport(0, 0, w, h);
}

void Water::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::width, Window::height);
}

void Water::loadShaderData(GLuint shaderProgram) {
	//Load dynamic data into shader
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), REFLECTION);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), REFRACTION);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudv"), DUDV);

	glUniform1i(glGetUniformLocation(shaderProgram, "quad_width"), width);
	glUniform1i(glGetUniformLocation(shaderProgram, "quad_height"), height);

	if (first_draw) {
		lastTime = glfwGetTime();
		moveFactor += WAVE_SPEED * lastTime;
		first_draw = false;
	}
	else {
		double curTime = glfwGetTime();
		moveFactor += WAVE_SPEED * (curTime - lastTime);
		lastTime = curTime;
	}
	moveFactor = fmod(moveFactor, 1.f);
	glUniform1f(glGetUniformLocation(shaderProgram, "moveFactor"), moveFactor);

	glm::mat4 model = glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
}

void Water::setActiveTextures() {
	//Bind textures to be used to active texture locations
	glActiveTexture(GL_TEXTURE0 + REFLECTION);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);

	glActiveTexture(GL_TEXTURE0 + REFRACTION);
	glBindTexture(GL_TEXTURE_2D, refractionTex);

	glActiveTexture(GL_TEXTURE0 + DUDV);
	glBindTexture(GL_TEXTURE_2D, dudv);
}

void Water::draw(GLuint shaderProgram) {
	loadShaderData(shaderProgram);
	setActiveTextures();

	//render the water quad
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
