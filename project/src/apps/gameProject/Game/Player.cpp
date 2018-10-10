
/// Internal Includes
#include "Player.hpp"
#include "../Engine/Input/Input.hpp"

/// External Includes

/// Std Includes

Player::Player()
	: cam(nullptr)
	, pos(0)
	, vel(0) {}

Player::~Player() {}

void Player::setCamera(CameraInput * _cam) {
	cam = _cam;
}

void Player::update(float dt) {

	if (cam) {
		Engine::Input::Input* in = Engine::Input::Input::GetInput();

		glm::vec3 dir = cam->direction();
		

		glm::vec3 tempVel;

		vel.y -= 9.82F * dt;

		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_FORWARD], false)) {
			tempVel += glm::normalize(glm::vec3(dir.x, 0, dir.z));
		}
		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_BACKWARD], false)) {
			tempVel -= glm::normalize(glm::vec3(dir.x, 0, dir.z));
		}
		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_LEFT], false)) {
			glm::vec3 left = cross(glm::vec3(0, 1, 0), dir);
			tempVel += glm::normalize(glm::vec3(left.x, 0, left.z));
		}
		if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_RIGHT], false)) {
			glm::vec3 right = cross(dir, glm::vec3(0, 1, 0));
			tempVel += glm::normalize(glm::vec3(right.x, 0, right.z));
		}

		if (in->wasPressedThisFrame(Engine::Input::KeyBindings[Engine::Input::KEYBIND_SPACE], false)) {
			vel += glm::vec3(0, 5, 0);
		}

		tempVel.y = vel.y;

		pos += tempVel * dt;

		if (pos.y < 0) {
			pos.y = 0;
			vel.y = 0;
		}

		cam->setCam(pos, dir);

	}
}

void Player::render() {}