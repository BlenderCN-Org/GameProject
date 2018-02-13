#include "Bullet.hpp"

#include "../../Engine/Input/Input.hpp"

Bullet::Bullet(float startX, float startY, float dir) {
	xPos = startX;
	yPos = startY;

	rot = dir;

	yVel = BULLET_SPEED * glm::cos(rot);
	xVel = BULLET_SPEED * glm::sin(rot);

	ttl = BULLET_TTL;

}

Bullet::~Bullet() {

}

void Bullet::update(float dt) {

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	int wndX = 0;
	int wndY = 0;
	Engine::Input::Input::GetInput()->getWindowSize(wndX, wndY);
	
	xPos -= xVel * dt;
	yPos += yVel * dt;
	
	if (yPos < -wndY) {
		yPos = float(wndY);
	}
	
	if (yPos > wndY) {
		yPos = float(-wndY);
	}
	
	if (xPos < -wndX) {
		xPos = float(wndX);
	}
	
	if (xPos > wndX) {
		xPos = float(-wndX);
	}

	ttl -= dt;

}

bool Bullet::dead() const {
	return ttl < 0.0F;
}