#include "ShadowMap.h"

ShadowMap::ShadowMap(int width, int height) : width(width), height(height) {
	//Create FBO
	glGenFramebuffers(1, &fbo);

	//Create depth texture
	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	

	/*glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++) {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
	}*/

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	glDrawBuffer(GL_NONE);

	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap() {
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &depth);
	}
}

void ShadowMap::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ShadowMap::bindDraw(GLenum CubeFace) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void ShadowMap::bindRead(GLenum TextureUnit) {
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

void ShadowMap::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::unbindDraw() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void ShadowMap::unbindRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}