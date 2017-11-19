/// Internal Includes

#include "Engine.hpp"
#include "Input/Input.hpp"
#include "Core/System.hpp"
#include "Graphics/Graphics.hpp"

CEngine::CEngine() : console(nullptr), renderEngine(nullptr), gameWindow(nullptr) {

	Engine::System::initSys();

	// Render Engine
	if (renderEngineLib.loadLibrary("RenderEngine.dll\0")) {
		//gConsole->print("Loaded RenderEngine.dll\n");
	} else {
		//gConsole->print("Failed to load Renderer\n");
		throw;
	}
	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	windowWidth = 1280;
	windowHeight = 720;

	RenderEngineCreateInfo reci;
	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = true;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;

	renderEngine = rProc();
	renderEngine->init(reci);
	renderEngine->updateViewPort(windowWidth, windowHeight);

	gRenderEngine = renderEngine;

	gameWindow = renderEngine->getMainWindow();

	console = new Core::Console();
	console->updateSize(windowWidth, windowHeight);

	Input::Input* input = Input::Input::GetInput();

	input->setupCallbacks(gameWindow);
	input->attachConsole(console);

	gameWindow->setWindowSize(windowWidth, windowHeight);
	gameWindow->setVsync(true);
	running = true;

}

CEngine::~CEngine() {

	delete console;
	
	gRenderEngine = nullptr;

	Input::Input::Release();

	renderEngine->release();
	renderEngineLib.unloadLibrary();

	Engine::System::deinitSys();

	console = nullptr;
	renderEngine = nullptr;
	gameWindow = nullptr;
}

void CEngine::registerDataParser(Interfaces::IDataParser* dataParser, DataParsersTypes parserType) {
	dataParsers[parserType].push_back(dataParser);
}

const bool CEngine::isRunning() const {
	return running;
}

void CEngine::update(const float dt) {
	Input::Input::GetInput()->reset();
	gameWindow->pollMessages();

	if (Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);
		renderEngine->updateViewPort(w, h);
		
		console->updateSize(w, h);

		windowHeight = h;
		windowWidth = w;

	}

	console->update(dt);
	running = gameWindow->isVisible();
}

void CEngine::clearBackBuffer() {
	renderEngine->clear();
}

void CEngine::clearDebug() {
	renderEngine->renderDebugFrame();
}

void CEngine::presentFrame() {

	console->render();

	gameWindow->swapBuffers();
}