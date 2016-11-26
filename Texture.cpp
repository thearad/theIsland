#include "Texture.h"

Texture::Texture(std::string path, std::string name) {
	shader_var = name;
	std::cout << shader_var << std::endl;
	std::cout << path.c_str() << std::endl;
	//Generate a texture
	glGenTextures(1, &id);

	//Set all data for GL_TEXTURE_2D to be sent to the generated texture
	glBindTexture(GL_TEXTURE_2D, id);

	//load img from path into texture
	unsigned char* img;
	img = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);

	//Set texture parameters
	/*TODO: current settings are for repeated textures. Change in future to be more
	flexible*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //s coordinates are repeated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //t coordinates are repeated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //when mapping up/down in size use lerp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //when mapping up/down in size use lerp

	//unbind GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() {
	return id;
}
std::string Texture::getShaderVar() {
	return shader_var;
}
/*
In heightmap:
	std::vector<Texture> textures;
	//arr of textures
	for each tex in arr_of_textures:
		textures.push_back(creat(tex, sampler+str(i)));

*/