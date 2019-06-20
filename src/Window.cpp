#include "Window.h"

// Behaviors
void Window::initilize() {
	this->win = glfwCreateWindow(this->sizeX, this->sizeY, window_name, NULL, NULL);
	if (windowInitilized()) {
		glfwSetWindowPos(win, this->posX, this->posY);
		glfwMakeContextCurrent(win);
	}
	else {
		glfwTerminate();
	}
}

// Getters
bool Window::windowInitilized() {
	if (!win) {
		return false;
	}
	return true;
}

GLFWwindow* Window::get_window() {
	return win;
}

int Window::get_width() {
	int width, height;
	glfwGetWindowSize(win, &width, &height);
	return width;
}

int Window::get_height() {
	int width, height;
	glfwGetWindowSize(win, &width, &height);
	return height;
}

float Window::get_aspect_ratio() {
	int width, height;
	glfwGetWindowSize(win, &width, &height);
	return width / height;
}