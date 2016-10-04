#ifndef GAME_HPP
#define GAME_HPP

#include "Chunk.hpp"

class Game
{

public:

	virtual ~Game();

	void init();
	
	void update(float dt);

private:

	GameObject* player;

};

#endif