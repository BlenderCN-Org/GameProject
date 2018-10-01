#ifndef CAMERAINPUT_HPP
#define CAMERAINPUT_HPP

/// Internal Includes
#include "../Engine/Input/Input.hpp"

/// External Includes
#pragma warning( push, 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

/// Std Includes

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

class CameraInput {
public:

	CameraInput()
		: input(nullptr)
		, viewMat(nullptr)
		, pos()
		, dir()
		, start()
		, playbackSpeed(1.0F)
		, centerX(0)
		, centerY(0)
		, angleH(0.0F)
		, angleV(0.0F)
		, mouseSpeed(0.0F)
		, camSpeed(1.0F)
		, rotH()
		, rotV() {}

	void init(glm::mat4* viewMat);
	void update(float dt, bool freecam = false);

	void setCam(glm::vec3 _pos, glm::vec3 _dir);

	glm::vec3 direction() const;

	void setCamSpeed(float speed);

	void setDefaultSpeed();

private:

	Engine::Input::Input* input;

	glm::mat4* viewMat;

	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 start;

	float playbackSpeed = 1.0f;

	int centerX, centerY;

	float angleH;
	float angleV;

	float mouseSpeed; //higher val = faster
	float camSpeed = 1.0F; // higher val = faster

	glm::mat3 rotH;
	glm::mat3 rotV;

	void mousepan(float x, float y);
	void keypan(float dt);
};

#endif