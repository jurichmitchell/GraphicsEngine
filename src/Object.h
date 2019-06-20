#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "Angel.h"
#include "Group.h"
#include "Face.h"
#include "TransformMaker.h"

class Object {
	private:
		std::string name;
		std::vector <vec3> vertices;
		std::vector <vec3> normals;
		std::vector <vec2> texture_coords;
		std::map<int, Face> faces;
		std::map<std::string, Material> materials;
		std::map<std::string, Group> groups;

		std::map<int, std::string> face_mapto_material;

		vec3 translation = vec3(0, 0, 0);
		vec3 scale = vec3(1, 1, 1);
		float rotation_x = 0.0f;
		float rotation_y = 0.0f;
		float rotation_z = 0.0f;

		// A data vector to hold all the vertices, their colors, and their normals
		// Data alternates in the following pattern
		//     vertex (3 x GLfloat)
		//     color (4 x GLfloat)
		//     emission (3 x GLfloat)
		//     normal (3 x GLfloat)
		std::vector<GLfloat> data;
		
		// Internal Behaviors
		float euclidean__distance(vec3 A, vec3 B);
		vec3 midpoint(vec3 A, vec3 B);
		int tessellate_recursive(Face face, std::string face_mtl_name, std::map<int, Face> &new_faces, int next_new_face_num, std::map<int, std::string> &new_f_m_map);

		//DEBUG
		void print_data(std::vector<GLfloat> data);
	public:
		// Constructors
		Object();
		Object(std::string name);

		// Setters
		void set_name(std::string name);
		void set_translation(vec3 translation);
		void set_scale(vec3 scale);
		void set_rotation_x(float angle);
		void set_rotation_y(float angle);
		void set_rotation_z(float angle);

		// Behaviors
		void add_group(Group group);
		void add_vertex(vec3 coordinates);
		void add_texture_coord(vec2 coordinates);
		void add_face(int face_num, Face face);
		void add_material(Material mtl);
		void add_face_material_mapping(int face_number, std::string material_name);

		// Precondition: The object has faces
		// Postcondition: All non-triangular faces in the object will be converted into triangles
		void triangulate();
		// Precondition: The object has faces
		// Postcondition: All large triangles in the object will be converted into small triangles
		void tessellate();
		void calculate_normals();
		void display(GLuint bufferID, GLuint program, mat4 viewMatrix);
		void print(std::string pre_print_text = "");
		
		// Getters
		int get_num_vertices();
		int get_num_text_coords();
		int get_num_faces();
		std::string get_name();
		void generate_vertex_data();
		bool empty();
};