#version 330 core

struct Material {
    vec3 ambient;
    sampler2D diffuse_tex;
    vec3 diffuse;
    sampler2D specular_tex;
    vec3 specular;

    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float intensity;
};

uniform DirectionalLight directionalLight;

// Camera's view position ( Direction is calculated knowing vertex' position )
uniform vec3 viewPos;

uniform Material material;

uniform bool diffuse_map;
uniform bool specular_map;

// Maximum number of point lights
const int numPointLights = 10;
uniform PointLight pointLights[numPointLights];

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = material.ambient*0.1;

    result += CalculateDirectionalLight(directionalLight, norm, viewDir);

    for(int x = 0; x < numPointLights; x++) {
        result += CalculatePointLight(pointLights[0], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse;

    if(diffuse_map)
        diffuse = diff * light.diffuse * vec3(texture(material.diffuse_tex, TexCoords));
    else
        diffuse = diff * light.diffuse * material.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if(specular_map)
        specular = spec * light.specular * vec3(texture(material.specular_tex, TexCoords));
    else
        specular = spec * light.specular * material.specular;


    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic*distance*distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse;

    if(diffuse_map)
        diffuse = (diff * light.diffuse + 0.05) * vec3(texture(material.diffuse_tex, TexCoords));
    else
        diffuse = diff * light.diffuse * material.diffuse;


    vec3 specular;

    if(specular_map)
        specular = spec * light.specular * vec3(texture(material.specular_tex, TexCoords));
    else
        specular = spec * light.specular * material.specular;


    return (ambient + diffuse + specular) * light.intensity;
}
