#include "Material.h"

// Constructors
Material::Material() {
	this->diffuse = Color();
	this->specular = Color();
	this->emission = Color();
	this->map_kd = "";
}

Material::Material(Color diffuse, Color specular, Color emission, std::string map_kd) {
	this->diffuse = diffuse;
	this->specular = specular;
	this->emission = emission;
	this->map_kd = map_kd;
}

// Setters
void Material::set_name(std::string name) {
	this->name = name;
}

void Material::set_diffuse(Color diffuse) {
	this->diffuse = diffuse;
}

void Material::set_specular(Color specular) {
	this->specular = specular;
}

void Material::set_emission(Color emission) {
	this->emission = emission;
}

void Material::set_map_kd(std::string map_kd) {
	this->map_kd = map_kd;
}

// Behaviors
void Material::print(std::string pre_print_text) {
	std::cout << pre_print_text << "Material: " << this->name << "\n";
	std::cout << pre_print_text << "\tDiffuse: " << diffuse.r << " " << diffuse.g << " " << diffuse.b << "\n";
	std::cout << pre_print_text << "\tSpecular: " << specular.r << " " << specular.g << " " << specular.b << "\n";
	std::cout << pre_print_text << "\tEmission: " << emission.r << " " << emission.g << " " << emission.b << "\n";
	std::cout << pre_print_text << "\tmap_kd: " << map_kd << "\n";
}

// Getters
std::string Material::get_name() {
	return name;
}

Color Material::get_diffuse() {
	return diffuse;
}

Color Material::get_specular() {
	return specular;
}

Color Material::get_emission() {
	return emission;
}

std::string Material::get_map_kd() {
	return map_kd;
}

bool Material::empty() {
	return (name.empty() && map_kd.empty());
}