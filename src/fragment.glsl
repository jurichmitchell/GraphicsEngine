#version 120
uniform vec4 lightColors[16];

varying vec3 vertexPosition;
varying vec3 normal;
varying vec4 mtlDiffuseColor;
varying vec4 emission;
varying vec3 lightDirection;
varying float num_lights;
varying vec3 lights_array[16];

vec4 ComputeAmbient();
vec4 ComputeDiffuse();
vec4 CalcDirLight(vec3 lightDir);
vec4 CalcPointLight(vec3 lightPosition, vec4 lightColor);

vec4 ComputeAmbient()
{
	// Define Ambient Strength
	float ambientStrength = 0.4;
	// Define ambient light color
	vec4 lightColor = vec4(0.5, 0.5, 0.5, 0);

	vec4 ambient = ambientStrength * lightColor;
	vec4 result = ambient * mtlDiffuseColor;
	
	return vec4(result.xyz, 1.0);
}

vec4 ComputeDiffuse()
{
	vec4 sumDiffuse = vec4(0, 0, 0, 0);
	
	sumDiffuse = CalcDirLight(lightDirection);
	
	for (int i = 0; i < num_lights; i++) {
		vec3 lightPosition = lights_array[i];
		vec4 lightColor = lightColors[i];
		sumDiffuse += CalcPointLight(lightPosition, lightColor);
	}
	
	return clamp(sumDiffuse, 0, 1);
}

vec4 CalcDirLight(vec3 lightDir)
{
	// Define the color of the directional light
	vec4 lightColor = vec4(0.5, 0.5, 0.5, 0);
	// Define the diffuse reflect coefficient
	float Kd = 1;

	vec3 reverseLightDir = normalize(-lightDir);
	float cosTheta = max(0, dot(reverseLightDir, normal));
	
	return (mtlDiffuseColor * lightColor * Kd * cosTheta);
}

vec4 CalcPointLight(vec3 lightPosition, vec4 lightColor)
{
	float Kd = 500;

	vec3 LightDir = normalize(vertexPosition - lightPosition);
	float distance = length(vertexPosition - lightPosition);
	
	vec3 reverseLightDir = -LightDir;
	float cosTheta = max(0, dot(reverseLightDir, normal));
	
	return (mtlDiffuseColor * lightColor * Kd * cosTheta / (1 + 0.2 * distance * distance));
}

void main()
{
	vec4 ambient = ComputeAmbient();
	vec4 diffuse = ComputeDiffuse();
	
	vec4 rgba = emission + clamp(ambient, 0, 1) + clamp(diffuse, 0, 1);

	gl_FragColor = clamp(vec4(rgba.xyz, mtlDiffuseColor.a), 0, 1);
}