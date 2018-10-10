#ifndef ASTEROID_HPP
#define ASTEROID_HPP

/// Internal Includes

/// External Includes
#pragma warning( push, 3 )
#include <glm/glm.hpp>
#pragma warning( pop )
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

enum class ASTEROID_SIZE {
	LARGE,
	MEDIUM,
	SMALL
};

class Asteroid {

public:

	Asteroid(ASTEROID_SIZE size, float ox = 0.0F, float oy = 0.0F);

	virtual ~Asteroid();

	void update(float dt);

	void render();

	bool collide(float x, float y);

	bool isDead() const;

	float xPos;
	float yPos;

	float rot;

	ASTEROID_SIZE mSize;

private:

	bool dead;

	float points[24 * 5];

	float xVel;
	float yVel;

	float rotVel;

	IMesh* asteroid;


};

#endif