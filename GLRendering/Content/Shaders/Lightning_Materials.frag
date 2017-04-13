#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 lightColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 color;

void main()
{
	// Ambient
	vec3 ambient = lightColor * material.ambient;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos); // Direction vector between the light source and the fragment's position
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * (lightColor * material.diffuse);
	
	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * (lightColor * material.specular);
	
	vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}