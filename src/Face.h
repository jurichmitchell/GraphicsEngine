#pragma once

#include <iostream>
#include <string>

class Face {
	// Represents one vertex in a face
	struct face_vertex {
		int vertex_i = 0; // The vertex index
		int texture_i = 0; // The texture coordinate index
		int normal_i = 0; // The normal index
	};

	private:
		face_vertex v1;
		face_vertex v2;
		face_vertex v3;
		face_vertex v4;
		int num_vertices;

	public:
		// Constructors
		Face();

		// Behaviors
		void add_face_vertex(int vertex_index, int texture_index, int normal_index);
		void print(std::string pre_print_text = "");

		// Getters
		int get_num_vertices();
		int get_vertex1_index();
		int get_vertex2_index();
		int get_vertex3_index();
		int get_vertex4_index();
		int get_texture1_index();
		int get_texture2_index();
		int get_texture3_index();
		int get_texture4_index();
		int get_normal1_index();
		int get_normal2_index();
		int get_normal3_index();
		int get_normal4_index();

		//Setters
		void set_vertex1_index(int index);
		void set_vertex2_index(int index);
		void set_vertex3_index(int index);
		void set_vertex4_index(int index);
		void set_texture1_index(int index);
		void set_texture2_index(int index);
		void set_texture3_index(int index);
		void set_texture4_index(int index);
		void set_normal1_index(int index);
		void set_normal2_index(int index);
		void set_normal3_index(int index);
		void set_normal4_index(int index);
};