#include "View.h"

// Behaviors
void View::setupview(int sizeX, int sizeY) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (float)sizeX, 0.0, (float)sizeY, -1.0, 1.0);

	glViewport(0, 0, sizeX, sizeY);
	glMatrixMode(GL_MODELVIEW);
}

void View::display(GLFWwindow* window, std::vector<Object> &objects, GLuint bufferID, GLuint program, mat4 viewMatrix, CubeMap& skybox, GLuint sky_vbo, GLuint sky_ibo, GLuint sky_program) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Use skybox shaders
	glUseProgram(sky_program);
	glDisable(GL_DEPTH_TEST);
	// Display skybox
	skybox.display(sky_vbo, sky_ibo, sky_program);

	// Use object shaders
	glUseProgram(program);
	glEnable(GL_DEPTH_TEST);
	// Iterate through all of the objects
	for (std::vector<Object>::iterator it = objects.begin(); it != objects.end(); it++) {
		// Call this object's display method to draw it to the screen
		it->display(bufferID, program, viewMatrix);
	}

	// Swap the back and front buffers to update the display
	glfwSwapBuffers(window);
}