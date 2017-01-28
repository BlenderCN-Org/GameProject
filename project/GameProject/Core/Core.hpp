#ifndef CORE_HPP
#define CORE_HPP

// project includes
#include "RenderEngine\IRenderEngine.hpp"
#include "Input\Input.hpp"
#include "ThreadManager.hpp"
#include "Settings\DisplaySettings.hpp"
#include "..\Loader\LibraryLoader.hpp"

#include "Memory\MemoryManager.hpp"
#include "Assets\AssetManager.hpp"

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

	void render(glm::mat4 viewMat);
	void swap();

	DisplaySettings* getDisplaySettings();

	MemoryManager* getMemoryManager();
	IRenderEngine* getRenderEngine();

	//@Temporary
	IShaderObject* getShaderObject();
	IShaderObject* getTextShaderObject();
	AssetManager* getAssetManager();

private:

	// core variables
	Lib renderEngineLib;
	MemoryManager mem;
	AssetManager assetManager;

	bool hadReset;
	bool running;
	int fps;

	IWindow* window;
	Console* console;

	IRenderEngine* renderEngine;
	Input* input;

	DisplaySettings disp;

	ThreadManager* thrdMgr;

	// temp
	IShaderObject* shaderObj;
	int vp = 0;
	int mdl = 0;

	IShaderObject* textShaderObj;

};

#endif