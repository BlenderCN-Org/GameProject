#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "Chunk.hpp"

class Game
{

public:

	virtual ~Game();

	void init();
	
	void setGameState(GameState newState);
	GameState getGameState() const;

	void update(float dt);

private:

	GameState state;
	GameObject* player;

};

#endif