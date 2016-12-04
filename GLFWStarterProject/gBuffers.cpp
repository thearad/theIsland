#include "gBuffers.h"

gBuffers::gBuffers(int width, int height) : width(width), height(height) {
	//Create FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Create GBuffer textures
	glGenTextures(1, &position);
	glGenTextures(1, &normal);
	glGenTextures(1, &color);
	glGenTextures(1, &light);
	glGenTextures(1, &depth);
	glGenTextures(1, &effect1);
	glGenTextures(1, &effect2);

	//Position
	glBindTexture(GL_TEXTURE_2D, position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Normal
	glBindTexture(GL_TEXTURE_2D, normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Color
	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Light buffer
	glBindTexture(GL_TEXTURE_2D, light);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Create first post process effect buffer
	glBindTexture(GL_TEXTURE_2D, effect1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Create second post process effect buffer
	glBindTexture(GL_TEXTURE_2D, effect2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Create depth texture
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	//Attach textures to FBO
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, light, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, effect1, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, effect2, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	for (int i = 0; i < 3; i++) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(3, drawBuffers);

	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

gBuffers::~gBuffers() {
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &position);
		glDeleteTextures(1, &normal);
		glDeleteTextures(1, &color);
		glDeleteTextures(1, &light);
		glDeleteTextures(1, &depth);
		glDeleteTextures(1, &effect1);
		glDeleteTextures(1, &effect2);
	}
}

GLuint gBuffers::getFBO() const {
	return fbo;
}

int gBuffers::getWidth() const {
	return width;
}

int gBuffers::getHeight() const {
	return height;
}

void gBuffers::setDrawBuffers() {
	glDrawBuffers(3, drawBuffers);
}

void gBuffers::setDrawLight() {
	glDrawBuffer(GL_COLOR_ATTACHMENT3);
}

void gBuffers::setDrawEffect() {
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
}

void gBuffers::setDrawNone() {
	glDrawBuffer(GL_NONE);
}

void gBuffers::setReadEffect() {
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void gBuffers::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void gBuffers::bindDraw() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void gBuffers::bindRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void gBuffers::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gBuffers::unbindDraw() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void gBuffers::unbindRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void gBuffers::setGeomTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, position);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, color);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depth);
}