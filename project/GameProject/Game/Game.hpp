#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "Chunk.hpp"

class Game
{

public:

	Game();
	virtual ~Game();

	void init();
	
	void update(float dt);

private:

	GameObject* player;
	
};

#endif