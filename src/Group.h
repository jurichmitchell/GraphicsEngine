#pragma once

#include <string>
#include <map>
#include <iostream>

#include "Material.h"

class Group {
	private:
		std::string name;
	public:
		// Constructors
		Group();

		// Setters
		void set_name(std::string name);

		// Behaviors
		void print(std::string pre_print_text = "");
		
		// Getters
		std::string get_name();

		bool empty();
};