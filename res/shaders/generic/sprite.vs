#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
	TexCoords = vertex.zw;
	//gl_Position = projection * model * vec4(vertex.xy,0.0, 1.0);
	gl_Position = view * projection * model * vec4(vertex.xy,0.0,1.0);
}