#pragma once

#include <stdio.h>

#include "Angel.h"

class TransformMaker {
	private:
		// Internal Behaviors
		static mat4 create_translation_matrix(vec3 translation);
		static mat4 create_scale_matrix(vec3 scale);
		static mat4 create_z_rot_matrix(GLfloat angle);
		static mat4 create_x_rot_matrix(GLfloat angle);
		static mat4 create_y_rot_matrix(GLfloat angle);
	public:
		static mat4 build_transform_matrix(vec3 translation, vec3 scale, GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z);
};