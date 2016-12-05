#include "HeightMap.h"

HeightMap::HeightMap(char* filename, GLfloat island_size) {
	ISLAND_SIZE = island_size;
	loadTextures();
	init();
	loadVertices(filename, island_size);
	bindData();
}

HeightMap::HeightMap(int width, int height, GLfloat island_size) {
	ISLAND_SIZE = island_size;
	loadTextures();
	init();
	genVertices(width, height, island_size);
	bindData();
}

void HeightMap::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vert);
	glGenBuffers(1, &VBO_norm);
	glGenBuffers(1, &VBO_tex);

	glGenBuffers(1, &EBO);
}

void HeightMap::bindData() {
	glBindVertexArray(VAO);

	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	// Vertex Normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0].x, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	// Texture Coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*texCoords.size(), &texCoords[0].x, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	//Reset buffer and vertex binds to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void HeightMap::loadTextures() {
	std::string path = "../textures/";
	std::vector<std::string> sTextureNames = {
		"ocean.jpg", "sand.jpg", "grass_4.jpg", "grass_3.jpg", "grass_rock.jpg", "snow.jpg"
	};
	for (int i = 0; i < sTextureNames.size(); i++) {
		textures.push_back(
			std::make_pair(Texture(path + sTextureNames[i]), "texSampler" + std::to_string(i))
		);
	}
}

void HeightMap::draw(GLuint shaderProgram) {
	//Send misc. data
	glm::mat4 model = glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);

	//Bind all available textures. Send texture bind location to corresponding uniform shader_var
	//See:https://www.opengl.org/wiki/Sampler_(GLSL) under Binding textures to samplers
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); //switches texture bind location to GL_TEXTURE(0+i)
		glBindTexture(GL_TEXTURE_2D, textures[i].first.getID()); //bind texture to active location
		glUniform1i(glGetUniformLocation(shaderProgram, textures[i].second.c_str()), i); //sets uniform sampler2D texSampleri's texture bind loc.
	}

	// Draw HeightMap
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//unbind textures
	for (int i = 0; i < textures.size()+1; i++) {
		glActiveTexture(GL_TEXTURE0 + i); //switches texture bind location to GL_TEXTURE(0+i)
		glBindTexture(GL_TEXTURE_2D, 0); //bind texture to active location
	}
}

void HeightMap::quickDraw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void HeightMap::drawNormals(GLuint shaderProgram) {
	glm::mat4 mvp = Window::P* Window::V * glm::mat4(1.f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void HeightMap::loadVertices(char* filename, GLfloat island_size) {
	unsigned char* data = loadPPM(filename, width, height);
	if (!data || width == 0 || height == 0) {
		std::cerr << "HeightMap::loadVertices failed - no data read from filename" << std::endl;
		return;
	}
	float y_mid = height / 2;
	float x_mid = width / 2;
	float max_width = island_size;

	int index_w = 0, index_h = 0;
	for (int h = floor(-height / 2); h < floor(height / 2); h++) {
		for (int w = floor(-width / 2); w < floor(width / 2); w++) {

			float v_h = data[(index_h*width + index_w) * 3]/255.f * 35.f - 2.f;

			float dist_x = pow(0.f - (float)w, 2);
			float dist_y = pow(0.f - (float)h, 2);
			float dist = sqrt(dist_x + dist_y);
			float dist_ratio = dist / max_width;

			float gradient = dist_ratio * dist_ratio;
			gradient = fmax(0.f, 1.f - gradient);

			vertices.push_back(glm::vec3(w, gradient*abs(v_h), h));
			index_w++;
		}
		index_h++;
		index_w = 0;
	}
	calcNormals();
	calcIndices();
	calcTexCoords();
}

/*
Generates a x by z heightmap vertices and texture coordinates
*/
void HeightMap::genVertices(int x, int z, GLfloat island_size) {
	srand((unsigned int)time(NULL));
	PerlinNoise pn(0.05, 0.2f, 35.f, 5, rand() % 100);

	width = x;
	height = z;

	float y_mid = height / 2;
	float x_mid = width / 2;
	float max_width = island_size;

	int index_w = 0, index_h=0;
	for (int h = -height / 2; h < height / 2; h++) {
		for (int w = -width / 2; w < width / 2; w++) {
			float dist_x = pow(0.f - (float)w, 2);
			float dist_y = pow(0.f - (float)h, 2);
			float dist_ratio = sqrt(dist_x + dist_y) / max_width;
			
			double v_h = pn.GetHeight(index_w, index_h);

			float gradient = 1.f - (dist_ratio * dist_ratio);

			if (gradient < 0.f) 
				v_h = -3*sqrt(abs(gradient));
			else
				v_h = gradient*abs(v_h);

			vertices.push_back(glm::vec3(w,v_h, h));
			index_w++;
		}
		index_h++;
		index_w = 0;
	}

	calcNormals();
	calcIndices();
	calcTexCoords();
}

void HeightMap::calcNormals() {
	//http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=24
	std::vector<std::vector<glm::vec3>> vNormals[2];
	for (int i = 0; i < 2; i++) {
		vNormals[i] = std::vector< std::vector<glm::vec3> >(height - 1, std::vector<glm::vec3>(width - 1));
	}
	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {
			glm::vec3 vTriangle0[] =
			{
				vertices[h*width + w],
				vertices[(h + 1)*width + w],
				vertices[(h + 1)*width + (w + 1)]
			};
			glm::vec3 vTriangle1[] =
			{
				vertices[(h + 1)*width + (w + 1)],
				vertices[h*width + (w + 1)],
				vertices[h*width + w]
			};

			vNormals[0][h][w] = getSurfaceNorm(vTriangle0[0], vTriangle0[1], vTriangle0[2]);
			vNormals[1][h][w] = getSurfaceNorm(vTriangle1[0], vTriangle1[1], vTriangle1[2]);
		}
	}
	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {
			glm::vec3 f_norm;

			if (h != 0 && w != 0)
				for (int k = 0; k<2; k++)
					f_norm += vNormals[k][h - 1][w - 1];

			// Look for upper-right triangles
			if (h != 0 && w != width - 1)
				f_norm += vNormals[0][h - 1][w];

			// Look for bottom-right triangles
			if (h != height - 1 && w != height - 1)
				for (int k = 0; k < 2; k++)
					f_norm += vNormals[k][h][w];

			// Look for bottom-left triangles
			if (h != height - 1 && w != 0)
				f_norm += vNormals[1][h][w - 1];

			normals.push_back(glm::normalize(f_norm));
		}
	}
}

void HeightMap::calcIndices() {
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
}

void HeightMap::calcTexCoords() {
	float fTextureU = float(width)*0.25f;
	float fTextureV = float(height)*0.25f;

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			float fScaleC = float(w) / float(width);
			float fScaleR = float(h) / float(height);

			//fScales range from [0-1]. fTexture = fraction of width & height
			texCoords.push_back(glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR));
		}
	}
}

void HeightMap::refresh(int width, int height, GLfloat island_size) {
	vertices.clear();
	normals.clear();
	indices.clear();
	if (width == 0 || height == 0 || island_size == 0)
		genVertices(this->width, this->height, this->ISLAND_SIZE);
	else {
		genVertices(width, height, island_size);
		this->width = width;
		this->height = height;
		this->ISLAND_SIZE = island_size;
	}
	bindData();
}

void HeightMap::refresh(char* filename, GLfloat island_size){
	vertices.clear();
	normals.clear();
	indices.clear();
	if (island_size == 0)
		loadVertices(filename, this->ISLAND_SIZE);
	else {
		loadVertices(filename, island_size);
		this->ISLAND_SIZE = island_size;
	}
	bindData();
}

void HeightMap::setIslandSize(GLfloat radius) {
	ISLAND_SIZE = radius;
}