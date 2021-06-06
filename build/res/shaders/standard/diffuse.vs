#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normalIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 normal;
out vec3 FragPos;

void main(){
    FragPos = vec3(model * vec4(pos, 1.0));
    normal = normalIn;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}