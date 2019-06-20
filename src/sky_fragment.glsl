#version 120

varying vec3 text_coords;

uniform samplerCube cubemap;

void main() {
	gl_FragColor = textureCube(cubemap, text_coords);
}