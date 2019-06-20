#include "Object.h"

// Internal Behaviors

float Object::euclidean__distance(vec3 A, vec3 B) {
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2) + pow((B.z - A.z), 2));
}

vec3 Object::midpoint(vec3 A, vec3 B) {
	return vec3((A.x + B.x) / 2, (A.y + B.y) / 2, (A.z + B.z) / 2);
}

// Constructors
Object::Object() {

}

Object::Object(std::string name) {
	this->name = name;
}

// Setters
void Object::set_name(std::string name) {
	this->name = name;
}

void Object::set_translation(vec3 translation) {
	this->translation = translation;
}

void Object::set_scale(vec3 scale) {
	this->scale = scale;
}

void Object::set_rotation_x(float angle) {
	this->rotation_x = angle;
}

void Object::set_rotation_y(float angle) {
	this->rotation_y = angle;
}

void Object::set_rotation_z(float angle) {
	this->rotation_z = angle;
}

// Behaviors
void Object::add_group(Group group) {
	groups.insert(std::pair<std::string, Group>(group.get_name(), group));
}

void Object::add_vertex(vec3 coordinates) {
	this->vertices.push_back(coordinates);
}

void Object::add_texture_coord(vec2 coordinates) {
	this->texture_coords.push_back(coordinates);
}

void Object::add_face(int face_num, Face face) {
	this->faces.insert(std::pair<int, Face>(face_num, face));
}

void Object::add_material(Material mtl) {
	this->materials.insert(std::pair<std::string, Material>(mtl.get_name(), mtl));
}

void Object::add_face_material_mapping(int face_number, std::string material_name) {
	this->face_mapto_material.insert(std::pair<int, std::string>(face_number, material_name));
}

void Object::triangulate() {
	std::map<int, Face> new_faces;
	int next_new_face_num = 1;
	std::map<int, std::string> new_f_m_map; // New face_mapto_material map

	// Iterate through all the faces
	for (std::map<int, Face>::iterator it = faces.begin(); it != faces.end(); it++) {
		// Get original_face_number
		int original_face_num = it->first;

		// If the face is a quadrilateral
		if (it->second.get_num_vertices() == 4) {

			// Determine the indices of the vertices that make up the face
			// Subtract 1 because the face indicies in the .obj file start counting from 1
			//     and the vector data type starts counting from 0
			int vertex1_index = it->second.get_vertex1_index() - 1;
			int vertex2_index = it->second.get_vertex2_index() - 1;
			int vertex3_index = it->second.get_vertex3_index() - 1;
			int vertex4_index = it->second.get_vertex4_index() - 1;
			// Get the vertices that comprise the face
			vec3 A = vertices.at(vertex1_index);
			vec3 B = vertices.at(vertex2_index);
			vec3 C = vertices.at(vertex3_index);
			vec3 D = vertices.at(vertex4_index);

			// Face1 will be comprised of vertices A, B, C
			Face face1 = Face();
			face1.add_face_vertex(it->second.get_vertex1_index(), it->second.get_texture1_index(), it->second.get_normal1_index()); // A
			face1.add_face_vertex(it->second.get_vertex2_index(), it->second.get_texture2_index(), it->second.get_normal2_index()); // B
			face1.add_face_vertex(it->second.get_vertex3_index(), it->second.get_texture3_index(), it->second.get_normal3_index()); // C
			// Face2 will be comprised of vertices A, C, D
			Face face2 = Face();
			face2.add_face_vertex(it->second.get_vertex1_index(), it->second.get_texture1_index(), it->second.get_normal1_index()); // A
			face2.add_face_vertex(it->second.get_vertex3_index(), it->second.get_texture3_index(), it->second.get_normal3_index()); // C
			face2.add_face_vertex(it->second.get_vertex4_index(), it->second.get_texture4_index(), it->second.get_normal4_index()); // D

			int face1num = next_new_face_num++;
			int face2num = next_new_face_num++;

			// Add Face1 and Face2 to the new_faces vector
			new_faces.insert(std::pair<int, Face>(face1num, face1));
			new_faces.insert(std::pair<int, Face>(face2num, face2));
			// If the original_face_num was in face_mapto_material
			std::map<int, std::string>::iterator mat_it = face_mapto_material.find(original_face_num);
			if (mat_it != face_mapto_material.end()) {
				// Add the new face numbers to the materials map
				new_f_m_map.insert(std::pair<int, std::string>(face1num, mat_it->second));
				new_f_m_map.insert(std::pair<int, std::string>(face2num, mat_it->second));
			}
		}
		else {
			int face_num = next_new_face_num++;

			// Add the current face to the new_faces map
			new_faces.insert(std::pair<int, Face>(face_num, it->second));
			// If the original_face_num was in face_mapto_material
			std::map<int, std::string>::iterator mat_it = face_mapto_material.find(original_face_num);
			if (mat_it != face_mapto_material.end()) {
				// Add the new face to the materials map
				new_f_m_map.insert(std::pair<int, std::string>(face_num, mat_it->second));
			}
		}
	}
	faces = new_faces;
	face_mapto_material = new_f_m_map;
}

void Object::tessellate() {
	std::map<int, Face> new_faces;
	int next_new_face_num = 1;
	std::map<int, std::string> new_f_m_map; // New face_mapto_material map

	// Iterate through all the faces
	for (std::map<int, Face>::iterator it = faces.begin(); it != faces.end(); it++) {
		std::string face_mtl_name = "";
		// Check if this face num is in face_mapto_material (the face has a correspoding material)
		std::map<int, std::string>::iterator mat_it = face_mapto_material.find(it->first);
		if (mat_it != face_mapto_material.end())
			face_mtl_name = mat_it->second;
		// Recurssively tessellate this face
		next_new_face_num = tessellate_recursive(it->second, face_mtl_name, new_faces, next_new_face_num, new_f_m_map);
	}
	faces = new_faces;
	face_mapto_material = new_f_m_map;
}

int Object::tessellate_recursive(Face face, std::string face_mtl_name, std::map<int, Face> &new_faces, int next_new_face_num, std::map<int, std::string> &new_f_m_map) {
	// Determine the indices of the vertices that make up the face
	int vertex_A_index = face.get_vertex1_index();
	int vertex_B_index = face.get_vertex2_index();
	int vertex_C_index = face.get_vertex3_index();

	// Get the vertices that comprise the face
	// Subtract 1 because the face indicies in the .obj file start counting from 1
	//     and the vector data type starts counting from 0
	vec3 A = vertices.at(vertex_A_index - 1);
	vec3 B = vertices.at(vertex_B_index - 1);
	vec3 C = vertices.at(vertex_C_index - 1);

	// Compute the length of each side
	float AB_length = euclidean__distance(A, B);
	float AC_length = euclidean__distance(A, C);
	float BC_length = euclidean__distance(B, C);

	// Tesselation base case
	//     Every side's length < 32
	//     Or at least one side is < 1
	if ((AB_length < 32 && AC_length < 32 && BC_length < 32)
		|| AB_length < 1 || AC_length < 1 || BC_length < 1) {
		int f_num = next_new_face_num++;

		// Add the current face to the new_faces map
		new_faces.insert(std::pair<int, Face>(f_num, face));

		// Add the face's material to the material map if it has one
		if (face_mtl_name != "")
			new_f_m_map.insert(std::pair<int, std::string>(f_num, face_mtl_name));
	}
	// General case
	else {
		// Calculate the mid points of each side
		vec3 midAB = midpoint(A, B);
		vec3 midAC = midpoint(A, C);
		vec3 midBC = midpoint(B, C);

		// Get the current vertex id of the last vertex in the "vertices" vector
		int last_vertex_id = vertices.size();

		// Add these mid points to the object's vertices vector
		// Get the vertex id of each new vertex added to the vector
		this->vertices.push_back(midAB);
		int vertex_midAB_index = ++last_vertex_id;
		this->vertices.push_back(midAC);
		int vertex_midAC_index = ++last_vertex_id;
		this->vertices.push_back(midBC);
		int vertex_midBC_index = ++last_vertex_id;

		// Create the 4 new faces based on the new vertex id's
		// Default their normal and texture_coordinate indices to 0
		Face face1 = Face();
		face1.add_face_vertex(vertex_A_index, 0, 0);
		face1.add_face_vertex(vertex_midAB_index, 0, 0);
		face1.add_face_vertex(vertex_midAC_index, 0, 0);
		Face face2 = Face();
		face2.add_face_vertex(vertex_midAB_index, 0, 0);
		face2.add_face_vertex(vertex_B_index, 0, 0);
		face2.add_face_vertex(vertex_midBC_index, 0, 0);
		Face face3 = Face();
		face3.add_face_vertex(vertex_midAC_index, 0, 0);
		face3.add_face_vertex(vertex_midBC_index, 0, 0);
		face3.add_face_vertex(vertex_C_index, 0, 0);
		Face face4 = Face();
		face4.add_face_vertex(vertex_midAB_index, 0, 0);
		face4.add_face_vertex(vertex_midBC_index, 0, 0);
		face4.add_face_vertex(vertex_midAC_index, 0, 0);
		
		// Call tessellate_recursive on the 4 new faces
		next_new_face_num = tessellate_recursive(face1, face_mtl_name, new_faces, next_new_face_num, new_f_m_map);
		next_new_face_num = tessellate_recursive(face2, face_mtl_name, new_faces, next_new_face_num, new_f_m_map);
		next_new_face_num = tessellate_recursive(face3, face_mtl_name, new_faces, next_new_face_num, new_f_m_map);
		next_new_face_num = tessellate_recursive(face4, face_mtl_name, new_faces, next_new_face_num, new_f_m_map);
	}
	return next_new_face_num;
}

void Object::calculate_normals() {
	// Empty the normals vector
	this->normals.clear();
	// Get the current normal id of the last normal in the normals vector
	//     Add one to start counting from 1 instead of 0
	int next_normal_id = normals.size() + 1;

	// Iterate through all the faces
	for (std::map<int, Face>::iterator it = faces.begin(); it != faces.end(); it++) {
		// Determine the indices of the vertices that make up the face
		int vertex_A_index = it->second.get_vertex1_index();
		int vertex_B_index = it->second.get_vertex2_index();
		int vertex_C_index = it->second.get_vertex3_index();

		// Get the vertices that comprise the face
		// Subtract 1 because the face indicies in the .obj file start counting from 1
		//     and the vector data type starts counting from 0
		vec3 A = vertices.at(vertex_A_index - 1);
		vec3 B = vertices.at(vertex_B_index - 1);
		vec3 C = vertices.at(vertex_C_index - 1);

		// Using A as our starting point, determine the two vector edges that stem from it
		vec3 AB = B - A;
		vec3 AC = C - A;

		vec3 normal = cross(AB, AC);

		//Add the normal to the normals vector
		normals.push_back(normal);
		int normal_index = next_normal_id;
		next_normal_id++;

		//Set the normal index for all vertices to this normal
		it->second.set_normal1_index(normal_index);
		it->second.set_normal2_index(normal_index);
		it->second.set_normal3_index(normal_index);
	}
}

void Object::print(std::string pre_print_text) {
	std::cout << pre_print_text << "Object: " << this->name << "\n";
	std::cout << pre_print_text << "\tTranslation: " << translation.x << ", " << translation.y << ", " << translation.z << "\n";
	std::cout << pre_print_text << "\tScale: " << scale.x << ", " << scale.y << ", " << scale.z << "\n";
	std::cout << pre_print_text << "\tRotation: " << rotation_x << ", " << rotation_y << ", " << rotation_z << "\n";
	//std::cout << pre_print_text << "\tNumber of vertices = " << get_num_vertices() << "\n";
	//std::cout << pre_print_text << "\tNumber of texture coordinates = " << get_num_text_coords() << "\n";
	//std::cout << pre_print_text << "\tNumber of groups = " << groups.size() << "\n";
	/*for (std::map<std::string, Group>::iterator it = groups.begin(); it != groups.end(); it++) {
		//std::cout << pre_print_text << "\t\t" << it->first << ":\n" << pre_print_text;
		it->second.print(pre_print_text + "\t");
	}
	for (std::vector<Face>::iterator it = faces.begin(); it != faces.end(); it++) {
		it->print();
	}
	for (std::map<std::string, Material>::iterator it = materials.begin(); it != materials.end(); it++) {
		std::cout << it->second.get_name() << "\n";
	}
	for (std::map<int, std::string>::iterator it = face_mapto_material.begin(); it != face_mapto_material.end(); it++) {
		std::cout << it->first << "\t"<< it->second << "\n";
	}
	std::cout << "Num faces: " << get_num_faces() << "\n";*/
}

void Object::display(GLuint bufferID, GLuint program, mat4 viewMatrix) {
	// Setup shader attributes for vertex position, color, emission, and normal
	GLuint vertLoc = glGetAttribLocation(program, "vPosition");
	GLuint colorLoc = glGetAttribLocation(program, "vColor");
	GLuint emissionLoc = glGetAttribLocation(program, "vEmission");
	GLuint normLoc = glGetAttribLocation(program, "vNormal");
	
	// Holds the reference to this object's vao
	GLuint vao;
	// The total number of vertices to be sent to the draw triangles function
	GLuint total_num_vertices = data.size() / 10;
	
	// Bind our buffer 'bufferID' to be used in place of GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data.front(), GL_STATIC_DRAW);

	// Generate 1 vertex array object (VAO) and store a reference to the point in "vao"
	glGenVertexArrays(1, &vao);
	// Tell openGL this is the vertex array object that will be used from now on
	glBindVertexArray(vao);

	// Calculate the size of each attribute based on the total number of values (3 vertex + 4 color + 3 emission + 3 normal) * the size of Glfloat
	int data_size = sizeof(GLfloat) * (3 + 4 + 3 + 3);

	// Enable the vertex attribute array
	// When enabled, the values in the generic vertex attribute array will be accessed and used for rendering
	//     when calls are made to vertex array commands
	glEnableVertexAttribArray(vertLoc);
	// Specify the location and data format of the array of generic vertex attributes to use when rendering
	//     The index of the generic vertex attribute to be modified: Start of vertLoc
	//     The number of components per attribute: 3
	//     The data type of each component in the array: GLfloat
	//     Do not normalize fixed-point data values when they are accessed.
	//     The byte offset between consecutive generic vertex attributes: data_size
	//     An offset from the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER: 0
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, data_size, 0);

	// Calculate the offset for the location of the next values based on the number of values already used
	GLvoid* offset = (GLvoid *)(sizeof(GLfloat) * 3);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, data_size, offset);
	
	offset = (GLvoid *)(sizeof(GLfloat) * 7);
	glEnableVertexAttribArray(emissionLoc);
	glVertexAttribPointer(emissionLoc, 3, GL_FLOAT, GL_FALSE, data_size, offset);

	offset = (GLvoid *)(sizeof(GLfloat) * 10);
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, data_size, offset);

	// Calculate the ModelMatrix
	mat4 modelMatrix = TransformMaker::build_transform_matrix(this->translation, this->scale,
		this->rotation_x, this->rotation_y, this->rotation_z);
	
	// Setup shader uniform for ModelView matrix
	GLuint gModelView = glGetUniformLocation(program, "ModelView");
	//Calculate the ModelViewMatrix
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// Specify the values of the uniform matrix for the current program object
	//     using the calculated value for modelViewMatrix
	glUniformMatrix4fv(gModelView, 1, GL_TRUE, modelViewMatrix);

	// Signify Alpha blending
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR, GL_DST_COLOR);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, total_num_vertices);
}

// Getters
int Object::get_num_vertices() {
	return this->vertices.size();
}

int Object::get_num_text_coords() {
	return this->texture_coords.size();
}

int Object::get_num_faces() {
	return this->faces.size();
}

std::string Object::get_name() {
	return this->name;
}

void Object::generate_vertex_data() {
	// A data vector to hold all the vertices, their colors, and their normals
	// Data alternates in the following pattern
	//     vertex (3 x GLfloat)
	//     color (4 x GLfloat)
	//     emission (3 x GLfloat)
	//     normal (3 x GLfloat)
	this->data.clear();

	// Iterate through every face
	for (std::map<int, Face>::iterator it = faces.begin(); it != faces.end(); it++) {
		// Get the three vertices for this face
		vec3 v1 = vertices.at(it->second.get_vertex1_index() - 1);
		vec3 v2 = vertices.at(it->second.get_vertex2_index() - 1);
		vec3 v3 = vertices.at(it->second.get_vertex3_index() - 1);

		// Get the material for this face if it exists
		std::string face_mtl_name = "";
		std::map<int, std::string>::iterator mat_it = face_mapto_material.find(it->first);
		if (mat_it != face_mapto_material.end())
			face_mtl_name = mat_it->second;

		// Add vertex 1 to the data vector
		data.push_back(v1.x);
		data.push_back(v1.y);
		data.push_back(v1.z);
		// If this face has a material
		if (face_mtl_name != "") {
			// Add the material diffuse to the data vector
			Material mtl = materials.at(face_mtl_name);
			Color diffuse = mtl.get_diffuse();
			data.push_back(diffuse.r);
			data.push_back(diffuse.g);
			data.push_back(diffuse.b);
			data.push_back(diffuse.a);

			// Add the material emission to the data vector
			Color emission = mtl.get_emission();
			data.push_back(emission.r);
			data.push_back(emission.g);
			data.push_back(emission.b);
		}
		else {
			// If there is no material for this face, add the color red to the data vector
			data.push_back(1);
			data.push_back(0);
			data.push_back(0);
			data.push_back(1);
			// Add the emission (0, 0, 0) to the data vector
			data.push_back(0);
			data.push_back(0);
			data.push_back(0);
		}
		// Add vertex 1 normal to the data vector
		vec3 normal = normals.at(it->second.get_normal1_index() - 1);
		data.push_back(normal.x);
		data.push_back(normal.y);
		data.push_back(normal.z);

		// Add vertex 2 to the data vector
		data.push_back(v2.x);
		data.push_back(v2.y);
		data.push_back(v2.z);
		// If this face has a material
		if (face_mtl_name != "") {
			// Add the material diffuse to the data vector
			Material mtl = materials.at(face_mtl_name);
			Color diffuse = mtl.get_diffuse();
			data.push_back(diffuse.r);
			data.push_back(diffuse.g);
			data.push_back(diffuse.b);
			data.push_back(diffuse.a);

			// Add the material emission to the data vector
			Color emission = mtl.get_emission();
			data.push_back(emission.r);
			data.push_back(emission.g);
			data.push_back(emission.b);
		}
		else {
			// If there is no material for this face, add the color red to the data vector
			data.push_back(1);
			data.push_back(0);
			data.push_back(0);
			data.push_back(1);
			// Add the emission (0, 0, 0) to the data vector
			data.push_back(0);
			data.push_back(0);
			data.push_back(0);
		}
		// Add vertex 2 normal to the data vector
		normal = normals.at(it->second.get_normal2_index() - 1);
		data.push_back(normal.x);
		data.push_back(normal.y);
		data.push_back(normal.z);

		// Add vertex 3 to the data vector
		data.push_back(v3.x);
		data.push_back(v3.y);
		data.push_back(v3.z);
		// If this face has a material
		if (face_mtl_name != "") {
			// Add the material diffuse to the data vector
			Material mtl = materials.at(face_mtl_name);
			Color diffuse = mtl.get_diffuse();
			data.push_back(diffuse.r);
			data.push_back(diffuse.g);
			data.push_back(diffuse.b);
			data.push_back(diffuse.a);

			// Add the material emission to the data vector
			Color emission = mtl.get_emission();
			data.push_back(emission.r);
			data.push_back(emission.g);
			data.push_back(emission.b);
		}
		else {
			// If there is no material for this face, add the color red to the data vector
			data.push_back(1);
			data.push_back(0);
			data.push_back(0);
			data.push_back(1);
			// Add the emission (0, 0, 0) to the data vector
			data.push_back(0);
			data.push_back(0);
			data.push_back(0);
		}
		// Add vertex 3 normal to the data vector
		normal = normals.at(it->second.get_normal3_index() - 1);
		data.push_back(normal.x);
		data.push_back(normal.y);
		data.push_back(normal.z);
	}
	//print_data(data);
}

void Object::print_data(std::vector<GLfloat> data) {
	for (int i = 0; i < data.size(); i+=13) {
		vec3 normal = vec3(data.at(i + 7), data.at(i + 8), data.at(i + 9));
		normal = normalize(normal);

		if (normal.x == 0 && normal.y == 0 && normal.z == 0) {
			std::cout << "vertex: (" << data.at(i) << ", " << data.at(i + 1) << ", " << data.at(i + 2) << ")\n";
			std::cout << "color: (" << data.at(i + 3) << ", " << data.at(i + 4) << ", " << data.at(i + 5) << ", " << data.at(i + 6) << ")\n";
			std::cout << "emission: (" << data.at(i + 7) << ", " << data.at(i + 8) << ", " << data.at(i + 9) << ")\n";
			std::cout << "normal: (" << data.at(i + 10) << ", " << data.at(i + 11) << ", " << data.at(i + 12) << ")\n";
		}

	}
}

bool Object::empty() {
	return (this->vertices.empty());
}