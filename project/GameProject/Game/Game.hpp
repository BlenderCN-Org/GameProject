#ifndef GAME_HPP
#define GAME_HPP

#include "Camera.hpp"
#include "EditorAccess.hpp"
#include "GameState.hpp"
#include "GameObject.hpp"
#include "Input/CameraInput.hpp"
#include "GUI/Gui.hpp"
#include "GUI/GuiComponents/GuiPanel.hpp"
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

	bool updateAndRender(float dt);

private:

	void render();

	void updateMenu(float dt);
	void updateEditor(float dt);
	void updateGame(float dt);
	void updateLoading(float dt);

	void tickFPS(float dt);

	// this stuff is really neeed
	Core* core = nullptr;
	Camera* cam = nullptr;

	GuiPanel* guiPanel;

	CameraInput camInput;

	bool running;
	float timepass;
	int fps;
	int ffps;

	GameState gstate;
	bool gamePaused;

	Scene* currentScene;

	// this is maybe needed in this class
	EditorAccess editAccess; // could be managed by core

	// this is not really needed in this class

	// can move gui class to core or something
	Gui* gui = nullptr;
	IFrameBuffer* gBuffer;
	// problably best if the scene handles the shaders it uses
	//IShaderObject* shdr;
	//int32_t vpLocation;
	//int32_t matLocation;
	//int32_t selectedLoc;
};

#endif