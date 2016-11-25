#include "HeightMap.h"

HeightMap::HeightMap() {
}

void HeightMap::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_vert);
	glGenBuffers(1, &VBO_norm);
	glGenBuffers(1, &EBO);

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
	
	//Reset buffer and vertex binds to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void HeightMap::draw(GLuint shaderProgram) {

	glm::mat4 model = Window::V * glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &model[0][0]);

	// Draw HeightMap
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void HeightMap::genMap(int x, int z) {
	PerlinNoise pn(2.5, 1.0, 4.0, 3, rand() % 100);
	width = x;
	height = z;
	//double hScale = 5 * (1.0 / height); //Scale height coordinates to [0-1].
	
	//Generate vertices
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			vertices.push_back(glm::vec3(w, pn.GetHeight(w, h), h));
		}
	}

	//Generate normals & indices
	glm::vec3 p1, p2, p3;
	for (int h = 0; h < height-1; h++) {
		for (int w = 0; w < width-1; w++) {
			p1 = vertices[h*width + w];
			p2 = vertices[(h + 1)*width + w];
			p3 = vertices[(h + 1)*width + (w + 1)];

			normals.push_back(getSurfaceNorm(p1, p2, p3));
			
			indices.push_back(h*width + w);
			indices.push_back((h + 1)*width + w);
			indices.push_back((h + 1)*width + (w + 1));

			p1 = vertices[(h + 1)*width + (w + 1)];
			p2 = vertices[h*width + (w + 1)];
			p3 = vertices[h*width + w];

			normals.push_back(getSurfaceNorm(p1, p2, p3));
		
			indices.push_back((h + 1)*width + (w + 1));
			indices.push_back(h*width + (w + 1));
			indices.push_back(h*width + w);
		}
	}
}