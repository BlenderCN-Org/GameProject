#ifndef SHIP_HPP
#define SHIP_HPP

#include <glm/glm.hpp>
#include <RenderEngine/IRenderEngine.hpp>

const float ROTATION = 1.3F;
const float ACCELERATION = 50.0F;
const float MAX_VELOCITY = 400.0F;

class Ship {

public:

	Ship();
	virtual ~Ship();

	void update(float dt);

	void render();

	bool getShootPoint(float &x, float &y);

	float xPos;
	float yPos;

	float rot;

private:
	
	float shootCD;
	bool shouldShoot;

	float xVel;
	float yVel;

	int lives;

	IMesh* shipMesh;

};

#endif