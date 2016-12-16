#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>

#include "SOIL.h"

class Texture {
private:
	int width, height;
	GLuint id;	//texture ID

public:
	/**
	Creates an empty gl texture

	Set glTexImage2D Params with optional arguments internalFormat, format, type
	Set glTexparameteri settings with optional argument vector<pair<GLenum, GLint>> texParams s.t. Glenum = pname and GLint = param

	Default settings will create an empty RGB texture
	**/
	Texture(
		int width, int height,
		//glTexImage2D Params
		GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE,
		//glTexParameteri settings
		std::vector<std::pair<GLenum, GLint>> texParams = {
			std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR),
			std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR),
			std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
			std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT)
		}
	);

	/**
	Creates a gl texture from the given path.

	Set glTexImage2D Params with optional arguments internalFormat, format, type
	Set glTexparameteri settings with optional argument vector<pair<GLenum, GLint>> texParams s.t. Glenum = pname and GLint = param

	Default settings will create repeating RGB texture
	**/
	Texture(
		std::string path,
		//glTexImage2D Params
		GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE,
		int soilFormat = SOIL_LOAD_RGB,
		//glTexParameteri settings
		std::vector<std::pair<GLenum, GLint>> texParams = { 
			std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
			std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT),
			std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR), 
			std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR) 
		}
	);

	~Texture();

	GLuint getID();
};