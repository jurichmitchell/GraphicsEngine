#include "Light.h"

// Constructors
Light::Light() {

}

// Setters
void Light::set_name(std::string name) {
	this->name = name;
}

void Light::set_color(Color color) {
	this->color = color;
}

void Light::set_translation(vec3 coordinates) {
	this->translation = coordinates;
}

// Getters
std::string Light::get_name() {
	return this->name;
}

Color Light::get_color() {
	return this->color;
}

vec3 Light::get_translation() {
	return this->translation;
}

bool Light::empty() {
	return (name.empty());
}