#include "Color.h"

Color::Color() {
	this->r = 0.0f;
	this->g = 0.0f;
	this->b = 0.0f;
	this->a = 1.0f;
}

Color::Color(float r, float g, float b, float a) {
	if (r < 0) this->r = 0.0f;
	else if (r > 1) this->r = 1.0f;
	else this->r = r;
	
	if (g < 0) this->g = 0.0f;
	else if (g > 1) this->g = 1.0f;
	else this->g = g;

	if (b < 0) this->b = 0.0f;
	else if (b > 1) this->b = 1.0f;
	else this->b = b;

	if (a < 0) this->a = 0.0f;
	else if (a > 1) this->a = 1.0f;
	else this->a = a;
}