
/// Internal Includess
#include "Game.hpp"

Game::Game(CEngine* _engine)
	: paused(false)
	, gameGui(nullptr)
	, engine(_engine) {
}

Game::~Game() {

	if (gameGui) {
		delete gameGui;
		gameGui = nullptr;
	}
}

void Game::update(float dt) {
	engine->update(dt);
}

void Game::render() {
	
}