#include "Model.h"

// Constructors
Model::Model() {

}

Model::Model(std::string name) {
	this->name = name;
}

// Setters
void Model::set_name(std::string name) {
	this->name = name;
}

// Behaviors
void Model::add_model(Model model) {
	this->models.insert(std::pair<std::string, Model>(model.get_name(), model));
}

void Model::add_object(Object object) {
	this->objects.insert(std::pair<std::string, Object>(object.get_name(), object));
}

void Model::add_light(Light light) {
	this->lights.insert(std::pair<std::string, Light>(light.get_name(), light));
}

void Model::print() {
	std::cout << "Model: " << this->name << "\n";
	std::cout << "Number of Objects = " << this->get_num_objects() << "\n";
	this->print_objects();
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		it->second.print_objects();
	}
	std::cout << "Number of Lights = " << this->get_num_lights() << "\n";
	this->print_lights();
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		it->second.print_lights();
	}
}

void Model::print_objects() {
	// Create an iterator for the objects map
	for (std::map<std::string, Object>::iterator it = objects.begin(); it != objects.end(); it++) {
		std::cout << "Object: " << it->second.get_name() << "\n";
		std::cout << "Number of vertices = " << it->second.get_num_vertices() << "\n";
		std::cout << "Number of texture coordinates = " << it->second.get_num_text_coords() << "\n";
	}
}

void Model::print_lights() {
	// Create an iterator for the lights map
	for (std::map<std::string, Light>::iterator it = lights.begin(); it != lights.end(); it++) {
		std::cout << "Light: " << it->second.get_name() << " " << it->second.get_color().r
			<< " " << it->second.get_color().g << " " << it->second.get_color().b << "\n";
	}
}

// Getters
std::string Model::get_name() {
	return this->name;
}

int Model::get_num_objects() {
	int num = 0;
	num += this->objects.size();
	// Create an iterator for the models map
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		// Add the number of objects in the model
		num += it->second.get_num_objects();
	}
	return num;
}

int Model::get_num_lights() {
	int num = 0;
	num += this->lights.size();
	// Create an iterator for the models map
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		// Add the number of objects in the model
		num += it->second.get_num_lights();
	}
	return num;
}

std::vector<Object> Model::get_objects() {
	std::vector<Object> rVal;
	// Create an iterator for the objects map
	for (std::map<std::string, Object>::iterator it = objects.begin(); it != objects.end(); it++) {
		rVal.push_back(it->second);
	}
	// Create an iterator for the models map
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		std::vector<Object> objs = it->second.get_objects();
		// Loop through objs vector and insert all into rVal vector
		for (int i = 0; i < objs.size(); i++)
			rVal.push_back(objs.at(i));
	}
	return rVal;
}

std::vector<Light> Model::get_lights() {
	std::vector<Light> rVal;
	// Create an iterator for the lights map
	for (std::map<std::string, Light>::iterator it = lights.begin(); it != lights.end(); it++) {
		rVal.push_back(it->second);
	}
	// Create an iterator for the models map
	for (std::map<std::string, Model>::iterator it = models.begin(); it != models.end(); it++) {
		std::vector<Light> lights = it->second.get_lights();
		// Loop through lights vector and insert all into rVal vector
		for (int i = 0; i < lights.size(); i++)
			rVal.push_back(lights.at(i));
	}
	return rVal;
}