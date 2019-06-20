#include "Face.h"

// Constructors
Face::Face() {
	num_vertices = 0;
}

// Behaviors
void Face::add_face_vertex(int vertex_index, int texture_index, int normal_index) {
	num_vertices++;
	switch (num_vertices) {
		case 1: v1 = { vertex_index, texture_index, normal_index };
			break;
		case 2: v2 = { vertex_index, texture_index, normal_index };
			break;
		case 3: v3 = { vertex_index, texture_index, normal_index };
			break;
		case 4: v4 = { vertex_index, texture_index, normal_index };
			break;
	}
}

void Face::print(std::string pre_print_text) {
	if (num_vertices == 3)
		std::cout << pre_print_text << "Face: " << v1.vertex_i << "/" << v1.texture_i << "/" << v1.normal_i << " "
			<< v2.vertex_i << "/" << v2.texture_i << "/" << v2.normal_i << " "
			<< v3.vertex_i << "/" << v3.texture_i << "/" << v3.normal_i << "\n";
	else
		std::cout << pre_print_text << "Face: " << v1.vertex_i << "/" << v1.texture_i << "/" << v1.normal_i << " "
			<< v2.vertex_i << "/" << v2.texture_i << "/" << v2.normal_i << " "
			<< v3.vertex_i << "/" << v3.texture_i << "/" << v3.normal_i << " "
			<< v4.vertex_i << "/" << v4.texture_i << "/" << v4.normal_i << "\n";
}

// Getters
int Face::get_num_vertices() {
	return num_vertices;
}

int Face::get_vertex1_index() {
	return this->v1.vertex_i;
}

int Face::get_vertex2_index() {
	return this->v2.vertex_i;
}

int Face::get_vertex3_index() {
	return this->v3.vertex_i;
}

int Face::get_vertex4_index() {
	return this->v4.vertex_i;
}

int Face::get_texture1_index() {
	return this->v1.texture_i;
}

int Face::get_texture2_index() {
	return this->v2.texture_i;
}

int Face::get_texture3_index() {
	return this->v3.texture_i;
}

int Face::get_texture4_index() {
	return this->v4.texture_i;
}

int Face::get_normal1_index() {
	return this->v1.normal_i;
}

int Face::get_normal2_index() {
	return this->v2.normal_i;
}

int Face::get_normal3_index() {
	return this->v3.normal_i;
}

int Face::get_normal4_index() {
	return this->v4.normal_i;
}

// Setters
void Face::set_vertex1_index(int index) {
	this->v1.vertex_i = index;
}

void Face::set_vertex2_index(int index) {
	this->v2.vertex_i = index;
}

void Face::set_vertex3_index(int index) {
	this->v3.vertex_i = index;
}

void Face::set_vertex4_index(int index) {
	this->v4.vertex_i = index;
}

void Face::set_texture1_index(int index) {
	this->v1.texture_i = index;
}

void Face::set_texture2_index(int index) {
	this->v2.texture_i = index;
}

void Face::set_texture3_index(int index) {
	this->v3.texture_i = index;
}

void Face::set_texture4_index(int index) {
	this->v4.texture_i = index;
}

void Face::set_normal1_index(int index) {
	this->v1.normal_i = index;
}

void Face::set_normal2_index(int index) {
	this->v2.normal_i = index;
}

void Face::set_normal3_index(int index) {
	this->v3.normal_i = index;
}

void Face::set_normal4_index(int index) {
	this->v4.normal_i = index;
}