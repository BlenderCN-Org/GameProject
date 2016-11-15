#ifndef CORE_HPP
#define CORE_HPP

#include "RenderEngine/IRenderEngine.hpp"
#include "Input.hpp"
#include "CameraInput.hpp"
#include "ThreadManager.hpp"

#include "Instance.hpp"
#include "../Game/Game.hpp"

#include "Settings\DisplaySettings.hpp"

#undef APIENTRY

#include "../Loader/LibraryLoader.hpp"

#include "Assets\ModelAsset.hpp"
#include "Assets\TextureAsset.hpp"

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

	// core variables
	Lib renderEngineLib;

	bool hadReset;
	bool running;
	int fps;

	IWindow* window;
	Console* console;
	Game* game;

	IRenderEngine* renderEngine;
	Input* input;
	CameraInput camInput;

	IShaderObject* shader;

	int vpLoc;
	int mdlLoc;
	int texLoc;
	
	ICamera* camera;

	DisplaySettings disp;

	ThreadManager thrdMgr;

	TextureAsset* ta;
	ModelAsset* ma;
	
	AssetLoadStruct mls;
	LoadInfo mli;

	AssetLoadStruct tls;
	LoadInfo tli;

	IMesh* planeMesh;
	IFrameBuffer* fbo;
	ITexture* texture;

};

#endif