#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

#include "../Core/Core.hpp"
#include "Input/CameraInput.hpp"

#include "..\Core\GUI\Text.hpp"

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
	void saveGame();
	void loadGame();

	int renderMenuItem(char* text, int length, int x, int y);
	void handleMenuEvent(int advance);
	void handleMenuEnter();

	void tickFPS(float dt);

	Core* core;
	Camera* cam;
	CameraInput camInput;

	bool running;
	float timepass;
	int fps;

	GameState gstate;
	GameObject* player;

	//@Temporary
	std::ifstream mapFile;
	IShaderObject* shObj;

	int vpLocation;
	int matLocation;

	IShaderObject* textShObj;
	int orthoLocation;
	int textLocation;
	int textureLocation;
	int colorLocation;


	Text* t = nullptr;

	// menu

	int menuItemCounter;
	int currentMenuItem;
	int nrMemuItems;
	int newGameIndex;
	int saveGameIndex;
	int loadGameIndex;
	int quitGameIndex;

	//@EndTemporary

};

#endif