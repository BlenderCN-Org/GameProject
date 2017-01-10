#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm\glm.hpp>

class Camera
{
public:

	virtual void init();
	virtual void release();

	virtual float* getViewMatrix();
	virtual float* getPerspectiveMatrix();
	virtual float* getOrthoMatrix();

//private:
	glm::mat4 viewMatrix;
	glm::mat4 perspective;
	glm::mat4 orthoMatrix;

};

#endif