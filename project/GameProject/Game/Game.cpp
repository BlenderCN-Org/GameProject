#include "Game.hpp"

Game::~Game()
{
	delete player;
}

void Game::init()
{
	player = new GameObject();
}

void Game::update(float dt)
{

}
