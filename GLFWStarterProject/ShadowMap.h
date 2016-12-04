#pragma once
#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "../Window.h"

class ShadowMap {
public:
	GLuint depth;
	GLuint shadowMap;

	ShadowMap(int width, int height);
	~ShadowMap();
	void bind();
	void bindDraw(GLenum CubeFace);
	void bindRead(GLenum TextureUnit);
	void unbind();
	void unbindDraw();
	void unbindRead();

protected:
	GLuint fbo;

	int width, height;
};

#endif