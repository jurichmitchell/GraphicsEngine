#version 120

attribute vec3 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
attribute vec3 vEmission;

uniform float theNumLights; // The number of point lights
uniform vec3 lightPositions[16]; // The positions of the point lights
uniform vec4 lightColors[16]; // The colors of the point lights

uniform mat4 View;
uniform mat4 ModelView; // The combination Model and View matrices
uniform mat4 Projection; // The projection matrix

varying vec3 vertexPosition; // The position of the vertex
varying vec3 normal; // The vertex's normal
varying vec4 mtlDiffuseColor; // The vertex's material diffuse color
varying vec4 emission; // The material's Ke
varying vec3 lightDirection; // The vector for directional light
varying float num_lights; // The number of point lights
varying vec3 lights_array[16]; // The transformed positions of the point lights

void main()
{
	mat4 ModelViewProjection = Projection * ModelView;
	
	// Transform vertex position
	gl_Position = ModelViewProjection * vec4(vPosition, 1);
	
	// Transform all light positions
	for (int i = 0; i < lights_array.length(); i++) {
		lights_array[i] = mat3(View) * lightPositions[i];
	}
	
	// Setup varying attributes to send to fragment shader
	vertexPosition = mat3(ModelView) * vPosition;
	//vertexPosition = vPosition;
	normal = normalize(mat3(ModelView) * vNormal);
	mtlDiffuseColor = vColor;
	emission = vec4(vEmission, 0);
	lightDirection = normalize(mat3(View) * vec3(0, -1, 0)); // The vector for directional light
	num_lights = theNumLights;
}