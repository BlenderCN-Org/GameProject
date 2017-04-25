#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

#include "../Core/Core.hpp"
#include "Input/CameraInput.hpp"

#include "..\Core\GUI\Text.hpp"

#include "../Core/GUI/Menu/Menu.hpp"

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

	//int renderMenuItem(char* text, int length);
	void handleMenuEvent(int advance);
	void handleMenuEnter();
	void toggleMenu();
	void toggleMenuTarget();

	void tickFPS(float dt);

	Core* core = nullptr;
	Camera* cam = nullptr;
	CameraInput camInput;

	bool running;
	float timepass;
	int fps;
	int ffps;

	GameState gstate;
	bool gameStarted;

	GameObject* player = nullptr;

	std::vector<GameObject> gameObjects;

	// mainMenu

	Menu* mainMenu = nullptr;

	//@Temporary
	std::ifstream mapFile;
	IShaderObject* shObj = nullptr;

	int vpLocation;
	int matLocation;

	IShaderObject* textShObj = nullptr;
	int orthoLocation;
	int textLocation;
	int textureLocation;
	int colorLocation;

	Text* t = nullptr;

	//@EndTemporary
};

#endif