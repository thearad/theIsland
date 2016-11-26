#include "util.h"
namespace util {
	glm::vec3 getSurfaceNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;
		return glm::normalize(glm::cross(U, V));
	}

	glm::vec3 getTrackballCoordinates(int width, int height, glm::vec2 point) {
		glm::vec3 v;
		v.x = (2.0*point.x - width) / width;
		v.y = (height - 2.0*point.y) / height;
		v.z = 0;
		double d = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));;
		d = (d < 1.f) ? d : 1.f;
		v.z = sqrt(1.001 - d*d);

		v = glm::normalize(v);

		return v;
	}

	//http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c
	unsigned char* loadPPM(char* filename, int &width, int &height) {
		char buff[16];
		FILE *fp;
		int c, rgb_comp_color;

		unsigned char* data;

		//open PPM file for reading
		fp = fopen(filename, "rb");
		if (!fp) {
			std::cerr << "Unable to open file " << filename << std::endl;
			return nullptr;
		}

		//read image format
		if (!fgets(buff, sizeof(buff), fp)) {
			perror(filename);
			fclose(fp);
			return nullptr;
		}

		//check the image format
		if (buff[0] != 'P' || buff[1] != '6') {
			std::cerr << "Invalid image format. Must be P6 "<< std::endl;
			fclose(fp);
			return nullptr;
		}

		//check for comments
		c = getc(fp);
		while (c == '#') {
			while (getc(fp) != '\n');
			c = getc(fp);
		}
		ungetc(c, fp);

		//read image size information
		if (fscanf(fp, "%d %d", &width, &height) != 2) {
			std::cerr << "Invalid image size." << std::endl;
			fclose(fp);
			return nullptr;
		}

		//read rgb component
		if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
			std::cerr << "Invalid rgb component" << std::endl;
			fclose(fp);
			return nullptr;
		}

		//check rgb component depth
		if (rgb_comp_color != RGB_COMPONENT_COLOR) {
			std::cerr << "Invalid rgb component. Does not have 8-bits." << std::endl;
			fclose(fp);
			return nullptr;
		}

		while (fgetc(fp) != '\n');
		
		data = new unsigned char[width * height * 3];
		if (fread(data, width*height * 3, 1, fp) != 1) {
			std::cerr << "Error while parsing ppm file." << std::endl;
			fclose(fp);
			delete[] data;
			return nullptr;
		}

		fclose(fp);
		return data;
	}
}
