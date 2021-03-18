#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	Camera(int sizex, int sizey);
	void SetDirection(glm::vec3 direction);
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	float FOV = 60;
private:	
	int resX;
	int resY;

};

#endif 