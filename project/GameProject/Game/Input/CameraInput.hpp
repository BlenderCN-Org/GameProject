#ifndef CAMERAINPUT_HPP
#define CAMERAINPUT_HPP

#include <glm\glm.hpp>
#include "../../Core/Input/Input.hpp"

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

class CameraInput {
public:

	void init(glm::mat4* viewMat);
	void update(float dt);

	void setCam(glm::vec3 _pos, glm::vec3 _dir);

private:

	Input* input;

	glm::mat4* viewMat;

	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 start;

	float playbackSpeed = 1.0f;

	int centerX, centerY;

	float angleH;
	float angleV;

	float mouseSpeed; //higher val = faster
	float camSpeed = 10.0f; // higher val = faster

	glm::mat3 rotH;
	glm::mat3 rotV;

	void mousepan(float x, float y);
	void keypan(float dt);
};

#endif