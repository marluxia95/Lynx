#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D img; // 2D sprite texture

void main()
{
	FragColor = texture(img, TexCoords);
}