#include "Camera.hpp"

using glm::mat4;

Camera::Camera() : viewMatrix(mat4()), perspective(mat4()) {}

Camera::~Camera() {}

float * Camera::getViewMatrix() {
	return &viewMatrix[0][0];
}

float * Camera::getPerspectiveMatrix() {
	return &perspective[0][0];
}

float * Camera::getOrthoMatrix() {
	return &orthoMatrix[0][0];
}