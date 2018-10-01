
/// Internal Includes
#include "Ship.hpp"

#include "../../Engine/Graphics/Graphics.hpp"
#include "../../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

Ship::Ship() : lives(0) {

	shipMesh = gRenderEngine->createMesh();
	shipMesh->init(MeshPrimitiveType::TRIANGLE);

	xPos = 0;
	yPos = 0;

	xVel = 0;
	yVel = 0;

	rot = 0;

	float points[] = {
		0.0F * 10.0F, 0.0F, +3.0F * 10.0F, 0.0F, 0.0F,
		1.5F * 10.0F, 0.0F, -3.0F * 10.0F, 0.0F, 0.0F,
		0.0F * 10.0F, 0.0F, -1.2F * 10.0F, 0.0F, 0.0F,

		+0.0F * 10.0F, 0.0F, +3.0F * 10.0F, 0.0F, 0.0F,
		+0.0F * 10.0F, 0.0F, -1.2F * 10.0F, 0.0F, 0.0F,
		-1.5F * 10.0F, 0.0F, -3.0F * 10.0F, 0.0F, 0.0F,

	};

	shipMesh->setMeshData(&points, sizeof(points), MeshDataLayout::VERT_UV);

	shootCD = -1.0F;
	shouldShoot = false;

}

Ship::~Ship() {
	shipMesh->release();
}

void Ship::update(float dt) {

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	int wndX = 0;
	int wndY = 0;
	Engine::Input::Input::GetInput()->getWindowSize(wndX, wndY);

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_RIGHT_ARROW])) {
		rot += (ROTATION * dt);
	}

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_LEFT_ARROW])) {
		rot -= (ROTATION * dt);
	}

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_UP_ARROW])) {

		float speed = (ACCELERATION * dt);

		yVel += speed * glm::cos(rot);
		xVel += speed * glm::sin(rot);
	}

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_SPACE])) {

		if (shootCD < 0.0F) {
			shouldShoot = true;
			shootCD = 1.0F;
		}

	}

	if (shootCD > 0.0F) {
		shootCD -= dt;
	}

	if (rot > 2 * 3.1415F) {
		rot -= (2 * 3.1415F);
	}

	if (rot < 0.0F) {
		rot += (2 * 3.1415F);
	}

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

}

void Ship::render() {

	shipMesh->bind();
	shipMesh->render();

}

bool Ship::getShootPoint(float &x, float &y) {
	bool ret = shouldShoot;
	
	if (shouldShoot) {
		float d = 30.0F;

		float yp = glm::cos(rot) * d;
		float xp = -glm::sin(rot) * d;

		x = xPos + xp;
		y = yPos + yp;
		shouldShoot = false;
	}

	return ret;
}