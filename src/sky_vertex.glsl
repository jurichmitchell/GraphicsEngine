#version 120

attribute vec3 vPosition; // position from mesh

uniform mat4 View;
uniform mat4 Projection;

varying vec3 text_coords;

void main()
{
	text_coords = vPosition;
	gl_Position = Projection * mat4(mat3(View)) * vec4(vPosition, 1.0);
}