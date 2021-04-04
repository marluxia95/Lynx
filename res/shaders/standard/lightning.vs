#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main() {
    FragPos = vec3(model * vec4(pos, 1.0));
    gl_Position = model * view * projection * vec4(pos, 1.0);
}