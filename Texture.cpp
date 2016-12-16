#include "Texture.h"


Texture::Texture(int width, int height, GLint internalFormat, GLenum format, GLenum type, std::vector<std::pair<GLenum, GLint>> texParams) {
	this->width = width;
	this->height = height;
	
	//Generate a texture
	glGenTextures(1, &id);

	//Set all data for GL_TEXTURE_2D to be sent to the generated texture
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

	//Set texture parameters
	for (int i = 0; i < texParams.size(); i++) {
		glTexParameteri(GL_TEXTURE_2D, texParams[i].first, texParams[i].second);
	}

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

Texture::Texture(std::string path, GLint internalFormat, GLenum format, GLenum type, int soilFormat, std::vector<std::pair<GLenum, GLint>> texParams) {
	std::cout << path.c_str() << std::endl;
	//Generate a texture
	glGenTextures(1, &id);

	//Set all data for GL_TEXTURE_2D to be sent to the generated texture
	glBindTexture(GL_TEXTURE_2D, id);

	//load img from path into texture
	unsigned char* img;
	img = SOIL_load_image(path.c_str(), &width, &height, 0, soilFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, img);
	SOIL_free_image_data(img);

	//Set texture parameters
	for (int i = 0; i < texParams.size(); i++) {
		glTexParameteri(GL_TEXTURE_2D, texParams[i].first, texParams[i].second);
	}

	//unbind GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() {
	return id;
}