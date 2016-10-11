#include "Camera.hpp"

void Camera::init()
{
	viewMatrix = glm::mat4();
	perspective= glm::mat4();
}

void Camera::release()
{
	delete this;
}

float * Camera::getViewMatrix()
{
	return &viewMatrix[0][0];
}

float * Camera::getPerspectiveMatrix()
{
	return &perspective[0][0];
}

float * Camera::getOrthoMatrix()
{
	return &orthoMatrix[0][0];
}
