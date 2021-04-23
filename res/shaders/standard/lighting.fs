#version 410 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight dirLight;

const int numPointLights = 1;
uniform PointLight pointLights[numPointLights];

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

// Without texture maps
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection){
    vec3 lightDirection = normalize(-light.direction);

    float diff = max(dot(normal, lightDirection), 0.0f);

    vec3 reflectDir = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);

}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse  = light.diffuse * diff;
    vec3 specular =  light.specular * spec;
    return (ambient + diffuse + specular);
}


void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);//CalculateDirectionalLight(dirLight, norm, viewDir);
    result += CalculatePointLight(pointLights[0], norm, FragPos, viewDir);      
     
    FragColor = vec4(result, 1.0);
}


