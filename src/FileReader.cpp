#include "FileReader.h"

Object FileReader::read_object(std::string path) {
	Object obj; // The data structure for the object file
	std::ifstream file_reader(path, std::ifstream::in); // The file reader
	std::string curr_line; // The current line being read
	Group curr_group; // Hold the information of the current group that is being read

	std::string curr_mtl = ""; // The current material being used for faces
	int curr_face_num = 1; // The current number of the next face to be read in

	// While the stream 'fileReader' has no errors
	while (std::getline(file_reader, curr_line)) {
		// DEBUG PRINT THE CURRENT LINE
		//std::cout << curr_line << "\n";

		// Split the current line by spaces into seperate words
		std::deque<std::string> words = std::deque<std::string>();
		split_string(curr_line, words);

		// If there was at least 1 word found in the current line
		if (!words.size() == 0) {

			// Pop the first word from the current line
			std::string word = words.front();
			words.pop_front();

			if (word == "mtllib") {
				// Read the next word as the .mtl file
				std::string mtllib = words.front();
				// Read in the materials contained in the mtl file
				std::map<std::string, Material> materials_map;
				read_materials(path, mtllib, materials_map);
				// Add the materials to the object
				for (std::map<std::string, Material>::iterator it = materials_map.begin(); it != materials_map.end(); it++)
					obj.add_material(it->second);
			}
			else if (word == "o" || word == "g") {
				// If curr_group is not empty (a group was previously read in before this one)
				if (!curr_group.empty()) {
					// Store the curr_group in the obj
					obj.add_group(curr_group);
				}
				// Reinitilize curr_group to make sure it is empty
				curr_group = Group();
				// Read the next word as the group name
				curr_group.set_name(words.front());
			}
			else if (word == "v") {
				// Read next 3 words as verticies
				float x = stof(words.front());
				words.pop_front();
				float y = stof(words.front());
				words.pop_front();
				float z = stof(words.front());
				words.pop_front();
				// Add the geometric vertex to the object
				obj.add_vertex(vec3(x, y, z));
			}
			else if (word == "vt") {
				// Read next 2 words as verticies
				float x = stof(words.front());
				words.pop_front();
				float y = stof(words.front());
				words.pop_front();
				// Add the texture vertex to the object
				obj.add_texture_coord(vec2(x, y));
			}
			else if (word == "f") {
				Face f;
				// Create an iterator for all the words in the buffer
				for (std::deque<std::string>::iterator it = words.begin(); it != words.end(); it++) {
					std::string number_buffer = "";
					int slash_count = 0; // The number of forward slashes that have been encountered
					int vertex_index;
					int texture_index;
					int normal_index;
					// For every letter in the word
					for (int i = 0; i < it->length(); i++) {
						// If the letter is a forward slash
						if (it->at(i) == '/') {
							slash_count++;
							// Check which type of index this is based on the number of slashes
							switch (slash_count) {
							case 1: vertex_index = (number_buffer != "") ? stoi(number_buffer) : 0; // number_buffer isn't zero, convert to int and store
									break;
								case 2: texture_index = (number_buffer != "") ? stoi(number_buffer) : 0;
									break;
							}
							number_buffer = "";
						}
						else {
							number_buffer = number_buffer + it->at(i);
						}
					}
					// Check which type of index the last number is based on the number of slashes
					switch (slash_count) {
						case 0: vertex_index = (number_buffer != "") ? stoi(number_buffer) : 0;
							break;
						case 1: texture_index = (number_buffer != "") ? stoi(number_buffer) : 0;
							break;
						case 2: normal_index = (number_buffer != "") ? stoi(number_buffer) : 0;
							break;
					}
					// Add the face_vertex to the face
					f.add_face_vertex(vertex_index, texture_index, normal_index);
				}
				// DEBUG: PRINT THE FACE
				//f.print();
				// Add the face to the object
				obj.add_face(curr_face_num, f);
				// If there is a material
				if (curr_mtl != "")
					// Add a mapping for this face to that material
					obj.add_face_material_mapping(curr_face_num, curr_mtl);
				// Incremenet the face_number
				curr_face_num++;
			}
			else if (word == "usemtl") {
				curr_mtl = words.front();
			}
		}
	}
	// In case the obj file weas empty, check to make sure we have read in a group at least once
	if (!curr_group.empty()) {
		// Add the last group to the object
		obj.add_group(curr_group);
	}
	
	// Convert all quadrilaterals into triangles
	obj.triangulate();

	// Convert all large triangles into small triangles
	obj.tessellate();

	// Calculate all the vertex normals
	obj.calculate_normals();

	// Generate vertex data to be sent to the GPU
	obj.generate_vertex_data();

	// DEBUG
	//obj.print();

	return obj;
}

void FileReader::read_materials(std::string obj_dir, std::string mtl_file_name, std::map<std::string, Material>& materials_map) {
	std::string mtl_path = ""; // String to hold the final mtl file path
	std::string buffer = ""; // Buffer for converting the obj_dir to the mtl_dir

	// Get the correct directory path for the mtl file
	// Step through each char of the cur_dir
	for (int x = 0; x < obj_dir.length(); x++) {
		// If the current char is a '\' or a  '/'
		if (obj_dir.at(x) == '\\' || obj_dir.at(x) == '/') {
			if (mtl_path.empty())
				mtl_path = buffer;
			else
				mtl_path = mtl_path + "\\" + buffer;
			buffer = "";
		}
		else {
			buffer = buffer + obj_dir.at(x);
		}
	}
	// Add the mtl_name to the dir
	mtl_path = mtl_path + "\\" + mtl_file_name;

	// DEBUG
	//std::cout << "Reading .mtl file " << mtl_file_name << "\n";
	//std::cout << "Filepath: " << mtl_path << "\n";

	// At this point we have the full directory path of the material file
	// Setup variables to read in mtl file
	std::ifstream file_reader(mtl_path, std::ifstream::in); // The file reader
	std::string curr_line; // The current line being read
	Material curr_mtl; // Hold the information of the current material that is being read

	// While the stream 'fileReader' has no errors
	while (std::getline(file_reader, curr_line)) {
		// DEBUG
		//std::cout << "\t" << curr_line << "\n";

		// Split the current line by spaces into seperate words
		std::deque<std::string> words = std::deque<std::string>();
		split_string(curr_line, words);

		// If there was at least 1 word found in the current line
		if (!words.size() == 0) {

			// Pop the first word from the current line
			std::string word = words.front();
			words.pop_front();

			if (word == "newmtl") {
				// If curr_mtl is not empty (a mtl was previously read in before this one)
				if (!curr_mtl.empty()) {
					// Store the curr_mtl in the map
					materials_map.insert(std::pair<std::string, Material>(curr_mtl.get_name(), curr_mtl));
				}
				// Reinitilize curr_mtl to make sure it is empty
				curr_mtl = Material();
				// Read the next word as the name of a new material
				curr_mtl.set_name(words.front());
			}
			else if (word == "Kd") {
				// Read the next 3 words as RGB values
				float r = stof(words.front());
				words.pop_front();
				float g = stof(words.front());
				words.pop_front();
				float b = stof(words.front());
				words.pop_front();
				// Default alpha value
				float a = 1.0f;
				// Check if there is an alpha value
				if (!words.empty())
					a = stof(words.front());
				// Add the diffuse color to the material
				curr_mtl.set_diffuse(Color(r, g, b, a));
			}
			else if (word == "Ks") {
				// Read the next 3 words as RGB values
				float r = stof(words.front());
				words.pop_front();
				float g = stof(words.front());
				words.pop_front();
				float b = stof(words.front());
				words.pop_front();
				// Default alpha value
				float a = 1.0f;
				// Check if there is an alpha value
				if (!words.empty())
					a = stof(words.front());
				// Add the specular color to the material
				curr_mtl.set_specular(Color(r, g, b, a));
			}
			else if (word == "Ke") {
				// Read the next 3 words as RGB values
				float r = stof(words.front());
				words.pop_front();
				float g = stof(words.front());
				words.pop_front();
				float b = stof(words.front());
				words.pop_front();
				// Default alpha value
				float a = 1.0f;
				// Check if there is an alpha value
				if (!words.empty())
					a = stof(words.front());
				// Add the emission color to the material
				curr_mtl.set_emission(Color(r, g, b, a));
			}
			else if (word == "map_Kd") {
				// Read the next word as the map_Kd path and add it to the Material
				curr_mtl.set_map_kd(words.front());
			}
		}
	}
	// In case the mtl file weas empty, check to make sure we have read in a material
	if (!curr_mtl.empty()) {
		// Add the last material to the map
		materials_map.insert(std::pair<std::string, Material>(curr_mtl.get_name(), curr_mtl));
	}

	// DEBUG
	//std::cout << "Number of materials found: " << materials_map.size() << "\n";
}

Model FileReader::read_model(std::string path) {
	Model mdl; // The data structure for the model file
	std::ifstream file_reader(path, std::ifstream::in); // The file reader
	std::string curr_line; // The current line being read
	Object curr_obj; // Hold the information of the current object that is being read
	Light curr_light; // Hold the information of the current light that is being read
	char last_line = '0'; // Keeps track if an object line or light line was the last line to be read (for the translation)

	// Get the name of the model file
	std::string path_split = path;
	std::replace(path_split.begin(), path_split.end(), '/', ' ');
	std::deque<std::string> path_words = std::deque<std::string>();
	split_string(path_split, path_words);
	mdl.set_name(path_words.back());

	// While the stream 'fileReader' has no errors
	while (std::getline(file_reader, curr_line)) {
		// DEBUG PRINT THE CURRENT LINE
		//std::cout << curr_line << "\n";

		// Split the current line by spaces into seperate words
		std::deque<std::string> words = std::deque<std::string>();
		split_string(curr_line, words);
		
		// If there was at least 1 word found in the current line
		if (!words.size() == 0) {
			
			// Pop the first word from the current line
			std::string word = words.front();
			words.pop_front();
			
			if (word == "obj") {
				// If curr_obj is not empty (an object was previously read in before this one)
				if (!curr_obj.empty()) {
					// Add the curr_obj to the model
					mdl.add_object(curr_obj);
				}
				// Read the next word as the name of the curr_obj
				std::string obj_name = words.front();
				words.pop_front();
				// Read the next word to create the curr_obj directory
				std::string obj_path = up_directory(path) + "/" + words.front();
				// Read in the obj
				curr_obj = read_object(obj_path);
				curr_obj.set_name(obj_name);
				// Update the last_line
				last_line = 'o';
			}
			else if (word == "trans") {
				// Read next 3 words as part of a 3-tuple
				float x = stof(words.front());
				words.pop_front();
				float y = stof(words.front());
				words.pop_front();
				float z = stof(words.front());
				words.pop_front();
				// Check if this translation belongs to an object or light
				if (last_line == 'o') {
					// Set the translation of the object
					curr_obj.set_translation(vec3(x, y, z));
				}
				else if (last_line == 'l') {
					// Set the translation of the light
					curr_light.set_translation(vec3(x, y, z));
				}
			}
			else if (word == "rotate") {
				// Read the next word as the angle
				float angle = stof(words.front());
				words.pop_front();
				// Read next 3 words as part of a 3-tuple
				float x = stof(words.front());
				words.pop_front();
				float y = stof(words.front());
				words.pop_front();
				float z = stof(words.front());
				words.pop_front();
				// Add the rotation to the object
				if (x == 1.0f)
					curr_obj.set_rotation_x(angle);
				if (y == 1.0f)
					curr_obj.set_rotation_y(angle);
				if (z == 1.0f)
					curr_obj.set_rotation_z(angle);
			}
			else if (word == "scale") {
				// Read next 3 words as part of a 3-tuple
				float x = stof(words.front());
				words.pop_front();
				float y = stof(words.front());
				words.pop_front();
				float z = stof(words.front());
				words.pop_front();
				// Set the scale of the object
				curr_obj.set_scale(vec3(x, y, z));
			}
			else if (word == "light") {
				// If curr_light is not empty (a light was previously read in before this one)
				if (!curr_light.empty()) {
					// Add the curr_light to the model
					mdl.add_light(curr_light);
				}
				// Reset the curr_light
				curr_light = Light();
				// Read the next word as the name of the light
				std::string light_name = words.front();
				words.pop_front();
				// Add the name to the curr_light
				curr_light.set_name(light_name);
				// Read the next 3 words as RGB values
				float r = stof(words.front());
				words.pop_front();
				float g = stof(words.front());
				words.pop_front();
				float b = stof(words.front());
				words.pop_front();
				// Default alpha value
				float a = 1.0f;
				// Check if there is an alpha value
				if (!words.empty())
					a = stof(words.front());
				// Add the color to the curr_light
				curr_light.set_color(Color(r, g, b, a));
				// Update the last_line
				last_line = 'l';
			}
			else if (word == "include") {
				std::string new_path = up_directory(path);
				new_path = new_path + "/" + words.front();
				Model included_model = read_model(new_path);
				mdl.add_model(included_model);
			}
		}
	}
	// In case the model file didn't have any objects, check to make sure we have read in a object at least once
	if (!curr_obj.empty()) {
		// Add the last object to the model
		mdl.add_object(curr_obj);
	}
	// In case the model file didn't have any lights, check to make sure we have read in a light at least once
	if (!curr_light.empty()) {
		// Add the last light to the model
		mdl.add_light(curr_light);
	}

	return mdl;
}

void FileReader::split_string(std::string string, std::deque<std::string>& words) {
	std::string buffer = ""; // Buffer to hold the char's as they are being stepped through
	// Step through each char of the current line
	for (int x = 0; x < string.length(); x++) {
		// If the current current char is a space
		if (string.at(x) == ' ') {
			// And if the the buffer is not currently empty
			if (!buffer.empty()) {
				words.push_back(buffer); // Add the current string from the buffer to the deque
				buffer = ""; // Clear the buffer
			}
		}
		// Else if the current char is not a space
		else {
			buffer = buffer + string.at(x); // Add the char to the buffer
		}
	}
	// If the buffer is not empty
	if (!buffer.empty()) {
		words.push_back(buffer); // Add the current string from the buffer to the deque
	}
}

std::string FileReader::up_directory(std::string path) {
	std::string new_path = ""; // String to hold the final directory path
	std::string buffer = ""; // Buffer for converting the path to the new_path
	
	// Step through each char of path
	for (int x = 0; x < path.length(); x++) {
		// If the current char is a '\' or a  '/'
		if (path.at(x) == '\\' || path.at(x) == '/') {
			if (new_path.empty())
				new_path = buffer;
			else
				new_path = new_path + "\\" + buffer;
			buffer = "";
		}
		else {
			buffer = buffer + path.at(x);
		}
	}
	return new_path;
}