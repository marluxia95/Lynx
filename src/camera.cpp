#include <stdio.h>
#include <glm/glm.hpp>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(int sizex, int sizey){
	resX = sizex;
	resY = sizey;
	pos = glm::vec3(0.0f);
}

glm::mat4 Camera::GetView(){
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(pos, pos + front, up);
	return view;
}

glm::mat4 Camera::GetProjection(){
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(FOV), (float)resX / (float)resY, 0.1f, 100.0f); 
	return projection;
}