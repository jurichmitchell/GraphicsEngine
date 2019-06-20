#pragma once

#include <string>
#include <map>
#include <vector>

#include "Object.h"
#include "Light.h"

class Model {
	private:
		std::string name;
		std::map<std::string, Model> models;
		std::map<std::string, Object> objects;
		std::map<std::string, Light> lights;

	public:
		// Constructors
		Model();
		Model(std::string name);

		// Setters
		void set_name(std::string name);

		// Behaviors
		void add_model(Model model);
		void add_object(Object object);
		void add_light(Light light);
		void print();
		void print_objects();
		void print_lights();

		// Getters
		std::string get_name();
		int get_num_objects();
		int get_num_lights();
		std::vector<Object> get_objects();
		std::vector<Light> get_lights();
};