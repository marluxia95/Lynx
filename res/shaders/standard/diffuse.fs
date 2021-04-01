#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 FragPos;

uniform Light light;  
uniform Material material;
uniform vec3 objectColor;
uniform vec3 viewPos;


out vec4 FragColor;

void main(){
	
    
    vec3 ambient = material.ambient * lightColor;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = material.specular * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;


    FragColor = vec4(result, 1.0);
}