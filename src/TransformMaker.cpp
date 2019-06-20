#include "TransformMaker.h"

void print_matrices(mat4 translation_matrix, mat4 scale_matrix, mat4 x_rot_matrix, mat4 y_rot_matrix, mat4 z_rot_matrix);

// Internal Behaviors

mat4 TransformMaker::create_translation_matrix(vec3 translation) {
	// Create identity matrix
	mat4 trans_mat = mat4();
	trans_mat[0].w = translation.x;
	trans_mat[1].w = translation.y;
	trans_mat[2].w = translation.z;
	return trans_mat;
}

mat4 TransformMaker::create_scale_matrix(vec3 scale) {
	// Create identity matrix
	mat4 scale_mat = mat4();
	scale_mat[0].x = scale.x;
	scale_mat[1].y = scale.y;
	scale_mat[2].z = scale.z;
	return scale_mat;
}

mat4 TransformMaker::create_z_rot_matrix(GLfloat angle) {
	GLfloat rad_angle = angle * (M_PI / 180.0f);
	// Create identity matrix
	mat4 rot_mat = mat4();
	rot_mat[0].x = cos(rad_angle);
	rot_mat[0].y = 0.0f - sin(rad_angle);
	rot_mat[1].x = sin(rad_angle);
	rot_mat[1].y = cos(rad_angle);
	return rot_mat;
}

mat4 TransformMaker::create_x_rot_matrix(GLfloat angle) {
	GLfloat rad_angle = angle * (M_PI / 180.0f);
	// Create identity matrix
	mat4 rot_mat = mat4();
	rot_mat[1].y = cos(rad_angle);
	rot_mat[1].z = 0.0f - sin(rad_angle);
	rot_mat[2].y = sin(rad_angle);
	rot_mat[2].z = cos(rad_angle);
	return rot_mat;
}

mat4 TransformMaker::create_y_rot_matrix(GLfloat angle) {
	GLfloat rad_angle = angle * (M_PI / 180.0f);
	// Create identity matrix
	mat4 rot_mat = mat4();
	rot_mat[0].x = cos(rad_angle);
	rot_mat[0].z = sin(rad_angle);
	rot_mat[2].x = 0.0f - sin(rad_angle);
	rot_mat[2].z = cos(rad_angle);
	return rot_mat;
}

// Behaviors

mat4 TransformMaker::build_transform_matrix(vec3 translation, vec3 scale, GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z) {
	mat4 translation_matrix = create_translation_matrix(translation);
	mat4 scale_matrix = create_scale_matrix(scale);
	mat4 rotation_matrix = create_z_rot_matrix(rotation_z) * create_y_rot_matrix(rotation_y) * create_x_rot_matrix(rotation_x);
	
	// DEBUG
	//print_matrices(translation_matrix, scale_matrix, create_x_rot_matrix(rotation_x), create_y_rot_matrix(rotation_y), create_z_rot_matrix(rotation_z));

	return translation_matrix * rotation_matrix * scale_matrix;
}

// DEBUG METHOD
void print_matrices(mat4 translation_matrix, mat4 scale_matrix, mat4 x_rot_matrix, mat4 y_rot_matrix, mat4 z_rot_matrix) {
	std::cout << "Translation Matrix\n"
		<< translation_matrix[0].x << "\t\t" << translation_matrix[0].y << "\t\t" << translation_matrix[0].z << "\t\t" << translation_matrix[0].w << "\n"
		<< translation_matrix[1].x << "\t\t" << translation_matrix[1].y << "\t\t" << translation_matrix[1].z << "\t\t" << translation_matrix[1].w << "\n"
		<< translation_matrix[2].x << "\t\t" << translation_matrix[2].y << "\t\t" << translation_matrix[2].z << "\t\t" << translation_matrix[2].w << "\n"
		<< translation_matrix[3].x << "\t\t" << translation_matrix[3].y << "\t\t" << translation_matrix[3].z << "\t\t" << translation_matrix[3].w << "\n";
	std::cout << "Scale Matrix\n"
		<< scale_matrix[0].x << "\t\t" << scale_matrix[0].y << "\t\t" << scale_matrix[0].z << "\t\t" << scale_matrix[0].w << "\n"
		<< scale_matrix[1].x << "\t\t" << scale_matrix[1].y << "\t\t" << scale_matrix[1].z << "\t\t" << scale_matrix[1].w << "\n"
		<< scale_matrix[2].x << "\t\t" << scale_matrix[2].y << "\t\t" << scale_matrix[2].z << "\t\t" << scale_matrix[2].w << "\n"
		<< scale_matrix[3].x << "\t\t" << scale_matrix[3].y << "\t\t" << scale_matrix[3].z << "\t\t" << scale_matrix[3].w << "\n";
	std::cout << "Z Rotation Matrix\n"
		<< z_rot_matrix[0].x << "\t\t" << z_rot_matrix[0].y << "\t\t" << z_rot_matrix[0].z << "\t\t" << z_rot_matrix[0].w << "\n"
		<< z_rot_matrix[1].x << "\t\t" << z_rot_matrix[1].y << "\t\t" << z_rot_matrix[1].z << "\t\t" << z_rot_matrix[1].w << "\n"
		<< z_rot_matrix[2].x << "\t\t" << z_rot_matrix[2].y << "\t\t" << z_rot_matrix[2].z << "\t\t" << z_rot_matrix[2].w << "\n"
		<< z_rot_matrix[3].x << "\t\t" << z_rot_matrix[3].y << "\t\t" << z_rot_matrix[3].z << "\t\t" << z_rot_matrix[3].w << "\n";
	std::cout << "X Rotation Matrix\n"
		<< x_rot_matrix[0].x << "\t\t" << x_rot_matrix[0].y << "\t\t" << x_rot_matrix[0].z << "\t\t" << x_rot_matrix[0].w << "\n"
		<< x_rot_matrix[1].x << "\t\t" << x_rot_matrix[1].y << "\t\t" << x_rot_matrix[1].z << "\t\t" << x_rot_matrix[1].w << "\n"
		<< x_rot_matrix[2].x << "\t\t" << x_rot_matrix[2].y << "\t\t" << x_rot_matrix[2].z << "\t\t" << x_rot_matrix[2].w << "\n"
		<< x_rot_matrix[3].x << "\t\t" << x_rot_matrix[3].y << "\t\t" << x_rot_matrix[3].z << "\t\t" << x_rot_matrix[3].w << "\n";
	std::cout << "Y Rotation Matrix\n"
		<< y_rot_matrix[0].x << "\t\t" << y_rot_matrix[0].y << "\t\t" << y_rot_matrix[0].z << "\t\t" << y_rot_matrix[0].w << "\n"
		<< y_rot_matrix[1].x << "\t\t" << y_rot_matrix[1].y << "\t\t" << y_rot_matrix[1].z << "\t\t" << y_rot_matrix[1].w << "\n"
		<< y_rot_matrix[2].x << "\t\t" << y_rot_matrix[2].y << "\t\t" << y_rot_matrix[2].z << "\t\t" << y_rot_matrix[2].w << "\n"
		<< y_rot_matrix[3].x << "\t\t" << y_rot_matrix[3].y << "\t\t" << y_rot_matrix[3].z << "\t\t" << y_rot_matrix[3].w << "\n";
	mat4 composite_rotation_matrix = z_rot_matrix * y_rot_matrix * x_rot_matrix;
	std::cout << "Composite Rotation Matrix\n"
		<< composite_rotation_matrix[0].x << "\t\t" << composite_rotation_matrix[0].y << "\t\t" << composite_rotation_matrix[0].z << "\t\t" << composite_rotation_matrix[0].w << "\n"
		<< composite_rotation_matrix[1].x << "\t\t" << composite_rotation_matrix[1].y << "\t\t" << composite_rotation_matrix[1].z << "\t\t" << composite_rotation_matrix[1].w << "\n"
		<< composite_rotation_matrix[2].x << "\t\t" << composite_rotation_matrix[2].y << "\t\t" << composite_rotation_matrix[2].z << "\t\t" << composite_rotation_matrix[2].w << "\n"
		<< composite_rotation_matrix[3].x << "\t\t" << composite_rotation_matrix[3].y << "\t\t" << composite_rotation_matrix[3].z << "\t\t" << composite_rotation_matrix[3].w << "\n";
	mat4 composite_transform = translation_matrix * composite_rotation_matrix * scale_matrix;
	std::cout << "Composite Transformation Matrix\n"
		<< composite_transform[0].x << "\t\t" << composite_transform[0].y << "\t\t" << composite_transform[0].z << "\t\t" << composite_transform[0].w << "\n"
		<< composite_transform[1].x << "\t\t" << composite_transform[1].y << "\t\t" << composite_transform[1].z << "\t\t" << composite_transform[1].w << "\n"
		<< composite_transform[2].x << "\t\t" << composite_transform[2].y << "\t\t" << composite_transform[2].z << "\t\t" << composite_transform[2].w << "\n"
		<< composite_transform[3].x << "\t\t" << composite_transform[3].y << "\t\t" << composite_transform[3].z << "\t\t" << composite_transform[3].w << "\n";
}