#pragma once

#define GLEW_STATIC  // Define that we are using the static version of GLEW
#include <GL/glew.h>  // must come before the other GL includes
#include <GLFW/glfw3.h> // GLFW

#include "stb_image.h"

class CubeMap {
	private:
		GLfloat cube_vertices[24] = {
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
		};
		// Cube indices for index buffer object
		GLushort cube_indices[24] = {
			0, 1, 2, 3,
			3, 2, 6, 7,
			7, 6, 5, 4,
			4, 5, 1, 0,
			0, 3, 7, 4,
			1, 2, 6, 5,
		};
		char* xpos;
		char* xneg;
		char* ypos;
		char* yneg;
		char* zpos;
		char* zneg;
		GLuint cubemap_texture;
	public:
		CubeMap();
		CubeMap(char* front, char* back, char* top, char* bottom, char* left, char* right);
		void setFront(char* front);
		void setBack(char* back);
		void setTop(char* top);
		void setBottom(char* bottom);
		void setLeft(char* left);
		void setRight(char* right);
		void create_texture();
		void display(GLuint vbo, GLuint ibo, GLuint program);
};