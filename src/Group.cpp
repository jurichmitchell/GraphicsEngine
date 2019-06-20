#include "Group.h"

// Constructors
Group::Group() {

}

// Setters
void Group::set_name(std::string name) {
	this->name = name;
}

// Behaviors
void Group::print(std::string pre_print_text) {
	std::cout << pre_print_text << "Group: " << this->name << "\n";
}

// Getters

std::string Group::get_name() {
	return this->name;
}

bool Group::empty() {
	return name.empty();
}