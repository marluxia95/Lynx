#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float strength = 0.1;
    vec3 ambientColor = strength * lightColor;

    vec3 result = ambientColor * objectColor;
    FragColor = vec4(result, 1.0);
}