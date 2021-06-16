#include <stdio.h>
#include <GL/glew.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h"

#include "logger.h"


Texture::Texture(const char* path, 
	int t_id,
	unsigned int texture_wrap_mode_s, 
	unsigned int texture_wrap_mode_t,
	unsigned int texture_minifying_filter,
	unsigned int texture_magnifying_filter) {
	stbi_set_flip_vertically_on_load(true);
	id = t_id;

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Wrapping / Filtering settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_mode_s);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_mode_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_minifying_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_magnifying_filter);


	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		log_error("Unable to load texture %s", path);
	}

	stbi_image_free(data);

}

Texture::Texture(const char* path, 
	int t_id) { 
	stbi_set_flip_vertically_on_load(true);
	//t_id++;
	id = t_id;

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Wrapping / Filtering settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		log_error("Unable to load texture %s", path);
	}

	stbi_image_free(data);

}

void Texture::use(){
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
}
