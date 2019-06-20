#pragma once

#include <vector>

#include "Object.h"
#include "CubeMap.h"

#define GLEW_STATIC  // Define that we are using the static version of GLEW
#include <GL/glew.h>  // must come before the other GL includes
#include <GLFW/glfw3.h> // GLFW

class View {
	public:
		// Behaviors
		void setupview(int sizeX, int sizeY);
		void display(GLFWwindow* window, std::vector<Object> &objects, GLuint bufferID, GLuint program, mat4 viewMatrix, CubeMap& skybox, GLuint sky_vbo, GLuint sky_ibo, GLuint sky_program);
};