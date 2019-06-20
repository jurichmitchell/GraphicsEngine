#pragma once

#include <string>
#include <iostream>

#include "Color.h"

class Material {
	private:
		std::string name;
		Color diffuse;
		Color specular;
		Color emission;
		std::string map_kd;
	public:
		// Constructors
		Material();
		Material(Color diffuse, Color specular, Color emission, std::string map_kd);

		// Setters
		void set_name(std::string name);
		void set_diffuse(Color diffuse);
		void set_specular(Color specular);
		void set_emission(Color emission);
		void set_map_kd(std::string map_kd);

		// Behaviors
		void print(std::string pre_print_text = "");

		// Getters
		std::string get_name();
		Color get_diffuse();
		Color get_specular();
		Color get_emission();
		std::string get_map_kd();
		bool empty();
};