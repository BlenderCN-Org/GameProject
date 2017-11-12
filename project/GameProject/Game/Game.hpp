#ifndef GAME_HPP
#define GAME_HPP

///Internal Includes
#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"

/// External Includes

/// Std Includes

class Game {
public:

	Game(CEngine* _engine);
	virtual ~Game();

	void update(float dt);

	void render();

private:

	bool paused;
	Engine::Graphics::CGui* gameGui;
	CEngine* engine;
};


#endif
