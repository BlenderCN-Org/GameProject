#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

#include "../Core/Core.hpp"
#include "Input/CameraInput.hpp"

class Game
{
public:

	Game();
	virtual ~Game();

	void init();

	bool isRunning() const;

	void update(float dt);
	void render();

private:

	void newGame();
	void loadGame();

	void tickFPS(float dt);

	Core* core;
	Camera cam;
	CameraInput camInput;

	bool running;
	float timepass;
	int fps;

	GameState gstate;
	GameObject* player;
};

#endif