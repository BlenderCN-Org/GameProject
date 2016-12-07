#include "Game.hpp"

#include "../Core/Input/Input.hpp"

Game::Game() : player(nullptr) {}

Game::~Game() {
	delete player;
}

void Game::init() {
	player = new GameObject();
}


void Game::update(float dt) {
	
}
