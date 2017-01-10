#ifndef CAMERAINPUT_HPP
#define CAMERAINPUT_HPP

#include <glm\glm.hpp>
#include "Input.hpp"

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

using namespace glm;

class CameraInput
{
public:

	void init(glm::mat4* viewMat);
	void update(float dt);

private:

	Input* input;

	glm::mat4* viewMat;

	vec3 pos;
	vec3 dir;
	vec3 start;

	float playbackSpeed = 1.0f;

	int centerX, centerY;

	float angleH;
	float angleV;

	float mouseSpeed; //higher val = faster
	float camSpeed = 10.0f; // higher val = faster

	mat3 rotH;
	mat3 rotV;

	void mousepan(float x, float y);
	void keypan(float dt);

	void setCam(vec3 _pos, vec3 _dir);
};

#endif