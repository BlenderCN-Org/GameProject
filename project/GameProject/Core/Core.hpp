#ifndef CORE_HPP
#define CORE_HPP

#include "RenderEngine/IRenderEngine.hpp"
#include "Input.hpp"
#include "CameraInput.hpp"
#include "ThreadManager.hpp"
#include "Assets\AssetManager.hpp"

#include "Instance.hpp"
#include "../Game/Game.hpp"

#include "Settings\DisplaySettings.hpp"

#include "../Loader/LibraryLoader.hpp"

#include "Assets\ModelAsset.hpp"
#include "Assets\TextureAsset.hpp"

#include "../GUI/MainMenu.hpp"
#include "../GUI/Text.hpp"

class Core
{
	
public:

	void init();
	void release();

	bool isRunning();
	bool hadGraphicsReset() const;
	
	void startWorkerThreads();
	void stopWorkerThreads();

	void setFPS(int fps);
	void update(float dt);

	void render();

	DisplaySettings* getDisplaySettings();

private:

	void updateMainMenu(float dt);
	void updateEditor(float dt);
	void updateGame(float dt);

	void enterNewGame();
	void loadGame();
	void enterEditor();

	// core variables
	Lib renderEngineLib;

	bool hadReset;
	bool running;
	int fps;

	IWindow* window;
	Console* console;
	Game* game;
	GameState state;
	GameState targetState;

	IRenderEngine* renderEngine;
	Input* input;
	CameraInput camInput;

	ICamera* camera;

	DisplaySettings disp;

	ThreadManager* thrdMgr;
	AssetManager* assetMgr;

	// temp

	IShaderObject* shader;
	IShaderObject* textShader;

	int vpLoc;
	int mdlLoc;
	int texLoc;

	int vp_textloc;
	int mdl_textloc;
	int tex_textloc;

	TextureAsset* ta;
	ModelAsset* ma;
	
	AssetLoadStruct mls;
	LoadInfo mli;

	AssetLoadStruct tls;
	LoadInfo tli;

	IMesh* planeMesh;
	IFrameBuffer* fbo;
	ITexture* texture;

	MainMenu mainMenu;

	IFont* font;
	Text* text;

	// end temp

};

#endif