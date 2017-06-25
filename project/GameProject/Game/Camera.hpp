#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <RenderEngine/IRenderEngine.hpp>

#include <glm\glm.hpp>

class Camera
{
public:

	Camera();
	~Camera();

	virtual float* getViewMatrix();
	virtual float* getPerspectiveMatrix();
	virtual float* getOrthoMatrix();

	glm::vec3 calculateMouseRay(float x, float y, IRenderEngine* renderEngine);
	glm::vec3 getPos();

	//private:
	glm::mat4 viewMatrix;
	glm::mat4 perspective;
	glm::mat4 orthoMatrix;
};

#endif