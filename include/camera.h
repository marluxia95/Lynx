#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum CameraType {
	CAMERA_ORTHOGRAPHIC,
	CAMERA_PERSPECTIVE
};

class Camera {
public:
	Camera(CameraType type, int sizex, int sizey);
	void SetDirection(glm::vec3 direction);
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	float FOV = 60;
	CameraType type;
private:	
	int resX;
	int resY;

};

#endif 