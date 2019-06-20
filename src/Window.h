#pragma once

#define GLEW_STATIC  // Define that we are using the static version of GLEW
#include <GL/glew.h>  // must come before the other GL includes
#include <GLFW/glfw3.h> // GLFW

class Window {
	private:
		GLFWwindow* win;
		int sizeX = 1280;
		int sizeY = 960;
		int posX = 200;
		int posY = 100;
		const char* window_name = "Assignemnt Final";
	public:
		// Behaviors
		void initilize();

		// Getters
		bool windowInitilized();
		GLFWwindow* get_window();
		int get_width();
		int get_height();
		float get_aspect_ratio();
};