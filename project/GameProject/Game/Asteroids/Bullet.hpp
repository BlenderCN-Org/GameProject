#ifndef BULLET_HPP
#define BULLET_HPP

const float BULLET_TTL = 5.0F;
const float BULLET_SPEED = 3.0F;

class Bullet {

public:

	Bullet(float startX, float startY, float dir);
	virtual ~Bullet();

	void update(float dt);

	bool dead() const;

	float xPos;
	float yPos;

	float ttl;

private:

	float rot;
	float xVel;
	float yVel;


};

#endif