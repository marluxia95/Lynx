#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera::Camera(CameraType type, int sizex, int sizey){
	resX = sizex;
	resY = sizey;
	type = type;
	pos = glm::vec3(0.0f);
	printf("Camera loaded\n");
}

glm::mat4 Camera::GetView(){
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(pos, pos + front, up);
	return view;
}

glm::mat4 Camera::GetProjection(){
	glm::mat4 projection = glm::mat4(1.0f);
	if(type == CAMERA_PERSPECTIVE){
		projection = glm::perspective(glm::radians(FOV), (float)resX / (float)resY, 0.1f, 100.0f); 
	}else{
		projection = glm::ortho(0.0f, (float)resX, (float)resY, 0.0f, -1.0f, 1.0f);
	}
	return projection;
}