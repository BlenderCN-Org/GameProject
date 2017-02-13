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

#include "GUI\Text.hpp"
//#include "System\Console.hpp"

#include <Editor_Wrapper\IEditor.hpp>

class Core
{
public:

	~Core();

	void init();
	void freeResources();

	bool isRunning();
	bool hadGraphicsReset() const;

	void startWorkerThreads();
	void stopWorkerThreads();

	void setFPS(int fps);
	void update(float dt);

	void render(glm::mat4 viewMat);
	void renderConsole();
	void swap();

	DisplaySettings* getDisplaySettings();

	MemoryManager* getMemoryManager();
	IRenderEngine* getRenderEngine();

	//@Temporary
	IShaderObject* getShaderObject();
	IShaderObject* getTextShaderObject();

	static int width;
	static int heigth;

private:

	// core variables
	Lib renderEngineLib;
	MemoryManager mem;
	AssetManager* assetManager;

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

	Text* text = nullptr;

	Lib editorLib;
	IEditor* editor;
};

#endif