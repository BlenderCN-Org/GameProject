#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "GameObject.hpp"

class Chunk : GameObject
{
public:

	virtual void init();
	virtual void update(float dt);

private:

	GameObject* gameObjects;
};

#endif