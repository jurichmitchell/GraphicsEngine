#include "CubeMap.h"

CubeMap::CubeMap() {
}

CubeMap::CubeMap(char* front, char* back, char* top, char* bottom, char* left, char* right) {
	zneg = front;
	zpos = back;
	ypos = top;
	yneg = bottom;
	xneg = left;
	xpos = right;
}

void CubeMap::setFront(char* front) {
	zneg = front;
}

void CubeMap::setBack(char* back) {
	zpos = back;
}

void CubeMap::setTop(char* top) {
	ypos = top;
}

void CubeMap::setBottom(char* bottom) {
	yneg = bottom;
}

void CubeMap::setLeft(char* left) {
	xneg = left;
}

void CubeMap::setRight(char* right) {
	xpos = right;
}

void CubeMap::create_texture() {
	// Create a texture
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &cubemap_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
	
	// Setup texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	unsigned char *data;
	
	// Load and add textures to cube map
	data = stbi_load(xpos, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	data = stbi_load(xneg, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	data = stbi_load(ypos, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	data = stbi_load(yneg, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	data = stbi_load(zpos, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	data = stbi_load(zneg, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void CubeMap::display(GLuint vbo_cube_vertices, GLuint ibo_cube_indices, GLuint program) {
	// Bind our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// Set up vertices attribute
	GLuint vertLoc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Bind our index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// Draw cube to screen
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}