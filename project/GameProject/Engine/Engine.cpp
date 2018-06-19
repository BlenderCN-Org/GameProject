/// Internal Includes

#include "Engine.hpp"
#include "Core/System.hpp"
#include "Core/LibraryLoader.hpp"
#include "Graphics/Graphics.hpp"
#include "Input/Input.hpp"

/// External Includes

/// Std Includes
#include <chrono>

const char* vertexShader = ""
"#version 410\n"
"\n"
"layout(location = 0) in vec3 vertexPos;\n"
"uniform mat4 vp;\n"
"uniform mat4 mdl;\n"
"\n"
"void main() {\n"
"	vec3 p = vertexPos;\n"
"	vec3 pos = (vec4(p, 1.0) * mdl).xyz;\n"
"	gl_Position = vp * vec4(pos, 1.0);\n"
"}\n"
"\n";

const char* fragmentShader = ""
"#version 410\n"
"\n"
"out float gl_FragDepth;\n"
"uniform float depthValue;\n"
"\n"
"void main() {\n"
"gl_FragDepth = depthValue;"
"}\n"
"\n";

CEngine::CEngine()
	: console(nullptr)
	, renderEngine(nullptr)
	, gameWindow(nullptr)
	, running(true) {

	console = new Engine::Core::Console();
	Engine::Core::gConsole = console;

	Engine::System::initSys();

	// this must be called before using renderEngine
	loadRenderEngine();
	loadSettings();

	createGameWindow();
	setupCursor();

	console->initGraphics(gui);
	console->updateSize(windowWidth, windowHeight);

	gameWindow->setCursorVisibility(false);

	setupInput(gameWindow, console);

	//gameWindow->lockCursor(true);

	createDepthShader();

	fullscreenQuad = new Engine::Graphics::FullscreenQuad();

	assetManager = new Engine::AssetManager();

	threadManager = new ThreadManager();

	setupPhysicsEngine();


}

CEngine::~CEngine() {


	delete groundPlane->shape;
	physEngine->freeStaticObject(groundPlane);

	delete threadManager;

	delete physEngine;

	delete assetManager;

	depthWriteShader->release();

	delete fullscreenQuad;

	delete console;

	delete cursor;
	delete gui;

	gRenderEngine = nullptr;

	Engine::Input::Input::Release();

	renderEngine->release();
	renderEngineLib.unloadLibrary();

	Engine::System::deinitSys();

	console = nullptr;
	renderEngine = nullptr;
	gameWindow = nullptr;
}

void CEngine::setCursor(char* cursorImgPath) {

	cursorTexture = assetManager->loadTexture(cursorImgPath);
	cursor->setTexture(cursorTexture);
}

void CEngine::close() {
	gameWindow->showWindow(false);
}

Engine::Graphics::CGui* CEngine::getGui() const {
	return gui;
}

bool CEngine::setAssetDataFolder(const char* folderPath) {
	bool success = false;
	if (Engine::System::folderExists(folderPath)) {
		gAssetDataPath = folderPath;
		success = true;
	}
	return success;
}

const bool CEngine::isRunning() const {
	return running;
}

void CEngine::update(const float dt) {
	Engine::Input::Input::GetInput()->reset();

	gameWindow->pollMessages();

	physEngine->update(1.0F / 60.0F);

	handleWindowSizeChange();

	console->update(dt);
	gui->update(dt);

	running = gameWindow->isVisible();
}

void CEngine::lockCursor(bool lock) {
	gameWindow->lockCursor(lock);
}

void CEngine::clearBackBuffer() {
	renderEngine->bindDefaultFrameBuffer();
	renderEngine->clear();
}

void CEngine::clearDebug() {
	renderEngine->bindDefaultFrameBuffer();
	renderEngine->clearDebug();
}

void CEngine::presentFrame() {
	gui->render();
	gameWindow->swapBuffers();
}

void CEngine::writeDepth(float depthValue, glm::mat4 vpMat, glm::mat4 mdl) {
	depthWriteShader->useShader();
	depthWriteShader->bindData(depthValueLoc, UniformDataType::UNI_FLOAT, &depthValue);
	depthWriteShader->bindData(depthVpMatLoc, UniformDataType::UNI_MATRIX4X4, &vpMat);
	depthWriteShader->bindData(depthMdlMatLoc, UniformDataType::UNI_MATRIX4X4, &mdl);
}

void CEngine::renderFullQuad() {
	fullscreenQuad->render();
}

Engine::Interfaces::IAssetManager* CEngine::getAssetManager() const {
	return assetManager;
}

PhysicsEngine* CEngine::getPhysEngine() {
	return physEngine;
}

ThreadManager* CEngine::getThreadManager() {
	return threadManager;
}

void CEngine::loadRenderEngine() {

	if (renderEngineLib.loadLibrary("RenderEngine.dll\0")) {
		Engine::Core::gConsole->print("Loaded RenderEngine.dll");
	} else {
		Engine::Core::gConsole->print("Failed to load Renderer");
		throw;
	}
}

void CEngine::loadSettings() {
	windowWidth = engineSettings.resolution().width;
	windowHeight = engineSettings.resolution().height;
}

void CEngine::createGameWindow() {

	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

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

	gameWindow->setWindowSize(windowWidth, windowHeight);
	gameWindow->setVsync(engineSettings.vSync());
}

void CEngine::setupInput(IWindow* window, Engine::Core::Console* console) {

	Engine::Input::Input* input = Engine::Input::Input::GetInput();

	input->setupCallbacks(window);
	input->attachConsole(console);
}

void CEngine::setupCursor() {

	gui = new Engine::Graphics::CGui();
	gui->setVisible(true);

	cursor = new Engine::Graphics::Gui::Cursor();
	cursor->setSize(25, 25);
	cursor->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	cursor->setVisible(true);

	gui->setCursor(cursor);
}

void CEngine::handleWindowSizeChange() {

	if (Engine::Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Engine::Input::Input::GetInput()->getWindowSize(w, h);

		renderEngine->updateViewPort(w, h);
		gameWindow->setWindowSize(w, h);

		console->updateSize(w, h);

		windowHeight = h;
		windowWidth = w;
	}
}

void CEngine::createDepthShader() {

	depthWriteShader = gRenderEngine->createShaderObject();
	depthWriteShader->init();

	depthWriteShader->setShaderCode(ShaderStages::VERTEX_STAGE, vertexShader);
	depthWriteShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fragmentShader);

	if (!depthWriteShader->buildShader()) {
		assert(0 && "Failed to build ClearDepth shader!");
	}

	depthValueLoc = depthWriteShader->getShaderUniform("depthValue");
	depthVpMatLoc = depthWriteShader->getShaderUniform("vp");
	depthMdlMatLoc = depthWriteShader->getShaderUniform("mdl");
}

void CEngine::setupPhysicsEngine() {
	PhysEngineCreateInfo peci;

	peci.threaded = true;
	peci.taskMgr = threadManager;
	//peci.maxTasks = System::getLogicalProcessorCount();
	peci.maxTasks = 8;

	physEngine = new PhysicsEngine(peci);

	groundPlane = physEngine->createStaticObject();

	PlaneShape* ps = new PlaneShape();
	groundPlane->shape = ps;
	ps->normal = glm::vec3(0, 1, 0);
	ps->distance = -5.0F;
}