#include "Core.hpp"
#include "System\Sys.hpp"

#include "Script\OcParser.hpp"

#include <thread>

void Core::init() {
	initSys();
	thrdMgr = nullptr;
	assetMgr = nullptr;

	running = true;
	hadReset = false;
	if ( renderEngineLib.loadLibrary("RenderEngine.dll\0") )
		printf("Loaded\n");
	else {
		printf("Failed to load\n");
		throw;
	}
	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	disp.setResolution(1280, 720);
	disp.setVsyncMode(VSyncMode::VSYNC_ON);
	disp.setFramerateLock(FramerateLock::FRAME_LOCK_NONE);
	disp.setFullscreenMode(FullscreenMode::WINDOWED);

	RenderEngineCreateInfo reci;
	reci.stype = SType::sRenderEngineCreateInfo;
	reci.createRenderWindow = true;
	reci.renderEngineType = RenderEngineType::eRenderOpenGL;
	reci.pNext = nullptr;

	renderEngine = rProc();
	renderEngine->init(reci);
	renderEngine->updateViewPort(1280, 720);

	window = renderEngine->getMainWindow();

	disp.setRenderEngine(renderEngine);
	disp.setWindow(window);
	disp.apply();

	input = Input::getInput();
	console = new Console();

	input->setupCallbacks(window);

	console->dispSettings = &disp;

	input->attachConsole(console);

	startWorkerThreads();

	assetMgr = new AssetManager();
	assetMgr->setThreadManager(thrdMgr);

	performParserTests();
	//parseOcFile("Data/Scripts/test.ocs");
}

void Core::release() {
	stopWorkerThreads();

	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	delete assetMgr;
	delete thrdMgr;

	delete console;

	renderEngine->release();

	renderEngineLib.unloadLibrary();

	input->release();

	deinitSys();
	delete this;
}

bool Core::isRunning() {
	return running;
}

bool Core::hadGraphicsReset() const {
	if ( console->reset ) {
		console->reset = false;
		return true;
	}
	return renderEngine->getGraphicsReset();
}

void Core::startWorkerThreads() {
	if ( !thrdMgr )
		thrdMgr = new ThreadManager();
	thrdMgr->startThreads(getLogicalProcessorCount());
}

void Core::stopWorkerThreads() {
	if ( thrdMgr )
		thrdMgr->stopThreads();
}

void Core::setFPS(int _fps) {
	fps = _fps;
}

void Core::update(float dt) {
	// reset input states, clear for next frame
	input->reset();
	// poll messages and update camera
	window->pollMessages();
	running = window->isVisible();

	// window size change event
	if ( input->sizeChange ) {
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);

		Resolution res = disp.getResolution();
		renderEngine->updateViewPort(res.width, res.height);
	}

	if ( input->consoleKeyWasPressed() ) {
		input->toggleConsole();
		console->setVisible(input->consoleIsActive());
	}

	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;
}

void Core::render() {
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	// swap
	window->swapBuffers();
}

DisplaySettings * Core::getDisplaySettings() {
	return &disp;
}