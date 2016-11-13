#include "Game.hpp"

Game::~Game()
{
	delete player;
}

void Game::init()
{
	state = GameState::eGameStage_MainMenu;
	player = new GameObject();
}

void Game::setGameState(GameState newState) {
	state = newState;
}

GameState Game::getGameState() const {
	return state;
}

void Game::update(float dt)
{
	if ( state == GameState::eGameState_Undefined ) {
		assert(0 && "Game state is undefined");
	}

	// perform game logic update
	if ( state == GameState::eGameState_PlayMode ) {

	}
}
