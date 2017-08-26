#ifndef GAME_HPP
#define GAME_HPP

#include "Camera.hpp"
#include "EditorAccess.hpp"
#include "GameState.hpp"
#include "GameObject.hpp"
#include "Input/CameraInput.hpp"
#include "GUI/Gui.hpp"
#include "Menu.hpp"

#include "../Core/Core.hpp"
#include "../Core/AssetManager.hpp"

#include <vector>

class Game {
public:

	Game();
	virtual ~Game();

	void init();

	bool isRunning() const;

	void updateAndRender(float dt);

private:

	void render();

	void updateMenu(float dt);
	void updateEditor(float dt);
	void updateGame(float dt);
	void updateLoading(float dt);

	void tickFPS(float dt);

	Core* core = nullptr;
	Camera* cam = nullptr;
	AssetManager* assetManager = nullptr;
	Gui* gui = nullptr;
	EditorAccess editAccess;
	CameraInput camInput;

	Scene currentGameScene;

	Scene* sceneThisFrame;

	Menu* menu;

	uint8_t editorEntry;
	uint8_t testEntry;

	bool running;
	float timepass;
	int fps;
	int ffps;

	GameState gstate;
	bool gamePaused;

	IShaderObject* shdr;
	int32_t vpLocation;
	int32_t matLocation;
	int32_t selectedLoc;

	// @todo not use vector
	std::vector<GameObject*> gameObjects;
};

#endif