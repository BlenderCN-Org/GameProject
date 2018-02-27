#ifndef PLAYER_HPP
#define PLAYER_HPP

/// Internal Includes
#include "GameObject.hpp"
#include "CameraInput.hpp"

class Player : public GameObject {

public:
	
	Player();
	virtual ~Player();

	void setCamera(CameraInput * cam);

	void update(float dt);

	void render();

private:

	CameraInput * cam;
	glm::vec3 pos;
	glm::vec3 vel;


};

#endif