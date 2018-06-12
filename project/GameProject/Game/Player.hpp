#ifndef PLAYER_HPP
#define PLAYER_HPP

/// Internal Includes
#include "IPlayer.hpp"
#include "CameraInput.hpp"

/// External Includes

/// Std Includes

class Player : public IPlayer {

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