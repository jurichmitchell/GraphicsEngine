#define GLEW_STATIC  // Define that we are using the static version of GLEW
#include <GL/glew.h>  // must come before the other GL includes
#include <GLFW/glfw3.h> // GLFW
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Angel.h"
#include "FileReader.h"
#include "Object.h"
#include "Model.h"
#include "Window.h"
#include "View.h"
#include "CubeMap.h"

// For loading skybox images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void err(int errcode, const char* desc);
void setup_event_handlers();
void key_press(GLFWwindow *pWin, int key, int scancode, int action, int mods);
void mouse_button(GLFWwindow *pWin, int button, int action, int mods);
void mouse_motion(GLFWwindow *pWin, double x, double y);
void resize_window(GLFWwindow *pWin, int width, int height);
void setup_data(char* argv[]);
void setup_skybox();

Window window;
GLuint program = 0;
std::vector<Object> objects;
GLuint bufferID;

CubeMap skybox;
GLuint sky_vbo;
GLuint sky_ibo;
GLuint sky_program;

vec3 eye = vec3(-10, 60, -200); // Pinhole point
vec3 at = vec3(10, 30, -100); // Looking towards point
vec3 up = vec3(0, 1, 0); // Looking up/down
mat4 projectionMatrix;
mat4 viewMatrix;
mat4 skyViewMatrix;

bool left_mouse_down = false;
double previous_mouse_xpos, previous_mouse_ypos;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Correct program usage: render model_path\n";
		return 1;
	}
	
	// Set the error call function
	glfwSetErrorCallback(err);
	// If we can't initilize the glfw library, exit
	if (!glfwInit())
		exit(1);

	// Create a Window object
	window = Window();
	window.initilize();
	// If the window didn't initilize, exit
	if (!window.windowInitilized())
		exit(1);

	// Setup the event handlers
	setup_event_handlers();

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		// Problem: glewInit failed, something is wrong.
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}

	program = InitShader("vertex.glsl", "fragment.glsl");
	glUseProgram(program);

	// Set polygon mode to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Enable the use of depth information
	glEnable(GL_DEPTH_TEST);

	// Setup the model file's data to be rendered
	setup_data(argv);

	setup_skybox();

	// Setup the view
	View view = View();
	view.setupview(window.get_width(), window.get_height());

	// Event loop
	do {
		view.display(window.get_window(), objects, bufferID, program, viewMatrix, skybox, sky_vbo, sky_ibo, sky_program);
		glfwWaitEvents();
	} while (!glfwWindowShouldClose(window.get_window()));

	return 0;
}

void err(int errcode, const char* desc) {
	fprintf(stderr, "%d: %s\n", errcode, desc);
	return;
}

void setup_event_handlers() {
	glfwSetKeyCallback(window.get_window(), key_press);
	glfwSetMouseButtonCallback(window.get_window(), mouse_button);
	glfwSetCursorPosCallback(window.get_window(), mouse_motion);
	glfwSetWindowSizeCallback(window.get_window(), resize_window);
}

void key_press(GLFWwindow *pWin, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		// Get the current polygon mode
		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		// Swap polygon modes
		if (polygonMode == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwDestroyWindow(pWin);
		glfwTerminate();
		exit(0);
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		int rate = 10;
		// The vector from the eye's position to where it is looking normalized
		vec3 eye_to_at = normalize(at - eye);
		// Multiply this vector by the rate of movement
		//     Now this vector is the change in movement
		eye_to_at = vec3(eye_to_at.x * rate, eye_to_at.y * rate, eye_to_at.z * rate);
		// Add the original eye point to the change in movement vector
		eye = vec3(eye.x + eye_to_at.x, eye.y + eye_to_at.y, eye.z + eye_to_at.z);
		// Add the original lookat point to the change in movement vector
		at = vec3(at.x + eye_to_at.x, at.y + eye_to_at.y, at.z + eye_to_at.z);
		// Recalculate the view matrix
		viewMatrix = LookAt(eye, at, up);
	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		int rate = 10;
		// The vector from the eye's position to where it is looking normalized
		vec3 eye_to_at = normalize(at - eye);
		// Multiply this vector by the rate of movement
		//     Now this vector is the change in movement
		eye_to_at = vec3(eye_to_at.x * rate, eye_to_at.y * rate, eye_to_at.z * rate);
		// Add the original eye point to the change in movement vector
		eye = vec3(eye.x - eye_to_at.x, eye.y - eye_to_at.y, eye.z - eye_to_at.z);
		// Add the original lookat point to the change in movement vector
		at = vec3(at.x - eye_to_at.x, at.y - eye_to_at.y, at.z - eye_to_at.z);
		// Recalculate the view matrix
		viewMatrix = LookAt(eye, at, up);
	}
}

void mouse_button(GLFWwindow *pWin, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Get previous mouse position
		glfwGetCursorPos(window.get_window(), &previous_mouse_xpos, &previous_mouse_ypos);
		left_mouse_down = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left_mouse_down = false;
	}
}

void mouse_motion(GLFWwindow *pWin, double x, double y) {
	if (left_mouse_down) {
		// Get current mouse pos
		double current_mouse_xpos, current_mouse_ypos;
		glfwGetCursorPos(window.get_window(), &current_mouse_xpos, &current_mouse_ypos);

		// Compute vector between eye and at
		vec3 delta = at - eye;

		// Compute the normal between delta and up (the x axis, pointing left)
		vec3 n1 = normalize(cross(vec3(0, 1, 0), delta));
		// Update the lookat position
		at += n1 * (previous_mouse_xpos - current_mouse_xpos);

		// Compute the normal between delta and n1 (the y axis, pointing up)
		vec3 n2 = normalize(cross(delta, n1));
		// Update the lookat position
		at += n2 * (previous_mouse_ypos - current_mouse_ypos);

		// Update ViewMatrix
		viewMatrix = LookAt(eye, at, up);
		// object shader
		glUseProgram(program);
		GLuint gView = glGetUniformLocation(program, "View");
		glUniformMatrix4fv(gView, 1, GL_TRUE, viewMatrix);
		// skybox shader
		glUseProgram(sky_program);
		gView = glGetUniformLocation(sky_program, "View");
		glUniformMatrix4fv(gView, 1, GL_TRUE, viewMatrix);
	}
}

void resize_window(GLFWwindow *pWin, int width, int height) {
	glViewport(0, 0, window.get_width(), window.get_height());

	// Update projection matrix
	projectionMatrix = Perspective(45, window.get_aspect_ratio(), 0.5, 1000.0);
	// Update projection matrix in object shader
	GLuint gProjection = glGetUniformLocation(program, "Projection");
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, projectionMatrix);
}

void setup_data(char* argv[]) {
	// Read in the model file
	Model model = FileReader::read_model(argv[1]);
	// Get objects from model
	objects = model.get_objects();
	// Get lights from model
	std::vector<Light> lights = model.get_lights();

	glUseProgram(program);

	// Setup shader uniforms for Projection and View matrices
	GLuint gProjection = glGetUniformLocation(program, "Projection");
	GLuint gView = glGetUniformLocation(program, "View");

	// Calculate the projection and view matrices
	projectionMatrix = Perspective(45, window.get_aspect_ratio(), 0.5, 1000.0);
	viewMatrix = LookAt(eye, at, up);

	// Specify the values of the uniform 4×4 matrices for the current program object
	//     using the calculated values for projection
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(gView, 1, GL_TRUE, viewMatrix);
	
	// Generate a bufferID
	glGenBuffers(1, &bufferID);
	// Bind our buffer 'bufferID' to be used in place of GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Create a vao to send data
	GLuint vao;
	// Generate 1 vertex array object (VAO) and store a reference to the point in "vao"
	glGenVertexArrays(1, &vao);
	// Tell openGL this is the vertex array object that will be used from now on
	glBindVertexArray(vao);


	//****************************
	// SETUP AND SEND LIGHT DATA
	//****************************
	// Get number of lights
	GLfloat num_lights = lights.size();
	// Get the positions (traslations) of every light
	std::vector<GLfloat> light_positions;
	for (int i = 0; i < lights.size(); i++) {
		vec3 translation = lights.at(i).get_translation();
		light_positions.push_back(translation.x);
		light_positions.push_back(translation.y);
		light_positions.push_back(translation.z);
	}
	// Get the colors of every light
	std::vector<GLfloat> light_colors;
	for (int i = 0; i < lights.size(); i++) {
		Color c = lights.at(i).get_color();
		light_colors.push_back(c.r);
		light_colors.push_back(c.g);
		light_colors.push_back(c.b);
		light_colors.push_back(1.0);
	}

	// Setup shader attribute for lights
	GLuint numLightLoc = glGetUniformLocation(program, "theNumLights");
	GLuint lightPosLoc = glGetUniformLocation(program, "lightPositions");
	GLuint lightColLoc = glGetUniformLocation(program, "lightColors");

	// Specify the values for the light data uniforms
	glUniform1f(numLightLoc, num_lights);
	glUniform3fv(lightPosLoc, num_lights, &light_positions.front());
	glUniform4fv(lightColLoc, num_lights, &light_colors.front());
}

void setup_skybox() {
	// Load skybox textures
	skybox.setFront("../SkyBox/cyclone-island_ft.tga");
	skybox.setBack("../SkyBox/cyclone-island_bk.tga");
	skybox.setTop("../SkyBox/cyclone-island_up.tga");
	skybox.setBottom("../SkyBox/cyclone-island_dn.tga");
	skybox.setLeft("../SkyBox/cyclone-island_lf.tga");
	skybox.setRight("../SkyBox/cyclone-island_rt.tga");
	skybox.create_texture();

	// Generate a VBO for the cube vertices
	glGenBuffers(1, &sky_vbo);

	// Generate an IBO buffer for cube indices
	glGenBuffers(1, &sky_ibo);

	// Initilize the skybox shaders
	sky_program = InitShader("sky_vertex.glsl", "sky_fragment.glsl");
	glUseProgram(sky_program);
	
	// Setup shader uniforms for Projection and View matrices
	GLuint gProjection = glGetUniformLocation(sky_program, "Projection");
	GLuint gView = glGetUniformLocation(sky_program, "View");
	glUniformMatrix4fv(gProjection, 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(gView, 1, GL_TRUE, viewMatrix);
}