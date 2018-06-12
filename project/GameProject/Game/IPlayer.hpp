#ifndef I_PLAYER_HPP
#define I_PLAYER_HPP

/// Internal Includes
#include "CameraInput.hpp"
#include "GameObject.hpp"

/// External Includes

/// Std Includes


class IPlayer : public GameObject {

public:
	virtual void update(float dt) = 0;
	virtual void setCamera(CameraInput * cam) = 0;

};

#endif