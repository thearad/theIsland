#pragma once
#ifndef GBUFFERS_H
#define GBUFFERS_H
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "C:\Users\anara\Desktop\CSE167Proj1_3\HA21\CSE167StarterCode2\OBJObject.h"

class gBuffers {
public:
	GLuint position, normal, color, depth, light, effect1, effect2;

	gBuffers(int widthIn, int heightIn);
	~gBuffers();

	GLuint getFBO() const;
	int getWidth() const;
	int getHeight() const;
	void setDrawBuffers();
	void setDrawLight();
	void setDrawEffect();
	void setDrawNone();
	//void setReadBuffer();
	void setReadEffect();
	void bind();
	void bindDraw();
	void bindRead();
	void unbind();
	void unbindDraw();
	void unbindRead();
	void setGeomTextures();

private:
	GLenum drawBuffers[3];

	GLuint fbo;

	int width, height;
};

#endif
