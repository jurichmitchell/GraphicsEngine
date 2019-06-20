#pragma once

#include "Angel.h"
#include "Color.h"

class Light {
	private:
		std::string name;
		Color color;
		vec3 translation;
	public:
		// Constructors
		Light();

		// Setters
		void set_name(std::string name);
		void set_color(Color color);
		void set_translation(vec3 coordinates);

		// Getters
		std::string get_name();
		Color get_color();
		vec3 get_translation();
		bool empty();
};