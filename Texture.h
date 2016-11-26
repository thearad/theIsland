#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "SOIL.h"

class Texture {
private:
	int width, height;
	GLuint id;	//texture ID
	std::string shader_var;	//shader variable this texture maps to

public:
	Texture();
	Texture(std::string, std::string);
	void creat(const GLchar* path, std::string name);

	GLuint getID();
	std::string getShaderVar();
};