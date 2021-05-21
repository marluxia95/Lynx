#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
	TexCoords = aPos.xy;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}