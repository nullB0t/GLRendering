#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

float ambientStrength = 0.1f;
float specularStrength = 0.5f;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// Ambient
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos); // Direction vector between the light source and the fragment's position
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}