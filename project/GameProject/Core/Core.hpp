#ifndef CORE_HPP
#define CORE_HPP

#include "RenderEngine/IRenderEngine.hpp"
#include "Input.hpp"
#include "CameraInput.hpp"

#include "Instance.hpp"
#include "../Game/Game.hpp"

#include "Settings\DisplaySettings.hpp"

#undef APIENTRY

#include "../Loader/LibraryLoader.hpp"

#include <thread>

#include "../Game/GUI/Gui.hpp"

#include "../Game/GUI/Editor/TextArea.hpp"

class Core
{
	
public:

	void init();
	void release();

	bool isRunning();
	bool hadGraphicsReset() const;

	void setFPS(int fps);
	void update(float dt);

	void render();

	DisplaySettings* getDisplaySettings();

private:

	// core variables
	bool hadReset;

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

	Lib renderEngineLib;

	bool running;

	// temp variables
	TextArea* consoleTextArea;

	IMesh* planeMesh;
	IMesh* triangleMesh;
	IFrameBuffer* fbo;

	ITexture* texture;

	bool toggle = true;
	int c;

	void* map;
	size_t mapSize;

	IText* text;
	IFont* font;

	std::thread* streamingThread;
	bool killThread;

	// gui stuff

	IText* mousePosText;

	TextArea* textArea;

	float areaSizeX;

};

#endif