/// Internal Includes

#include "Engine.hpp"
#include "Core/System.hpp"
#include "Core/LibraryLoader.hpp"
#include "Graphics/Graphics.hpp"
#include "Input/Input.hpp"

/// External Includes

/// Std Includes
#include <chrono>

const char* vs = ""
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

const char* fs = ""
"#version 410\n"
"\n"
"out float gl_FragDepth;\n"
"uniform float depthValue;\n"
"\n"
"void main() {\n"
"gl_FragDepth = depthValue;"
"}\n"
"\n";

CEngine::CEngine() : console(nullptr), renderEngine(nullptr), gameWindow(nullptr) {

	Engine::System::initSys();

	// Render Engine
	if (renderEngineLib.loadLibrary("RenderEngine.dll\0")) {
		//console->print("Loaded RenderEngine.dll\n");
	} else {
		//gConsole->print("Failed to load Renderer\n");
		throw;
	}

	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	windowWidth = engineSettings.resolution().width;
	windowHeight = engineSettings.resolution().height;

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

	gameWindow->setCursorVisibility(false);

	console = new Core::Console();
	console->updateSize(windowWidth, windowHeight);

	Input::Input* input = Input::Input::GetInput();

	input->setupCallbacks(gameWindow);
	input->attachConsole(console);

	gameWindow->setWindowSize(windowWidth, windowHeight);
	gameWindow->setVsync(engineSettings.vSync());
	//gameWindow->lockCursor(true);
	running = true;

	depthWriteShader = gRenderEngine->createShaderObject();
	depthWriteShader->init();

	depthWriteShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs);
	depthWriteShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs);

	if (!depthWriteShader->buildShader()) {
		assert(0 && "Failed to build ClearDepth shader!");
	}

	depthValueLoc = depthWriteShader->getShaderUniform("depthValue");
	depthVpMatLoc = depthWriteShader->getShaderUniform("vp");
	depthMdlMatLoc = depthWriteShader->getShaderUniform("mdl");

	fullQuad = gRenderEngine->createMesh();
	fullQuad->init(MeshPrimitiveType::TRIANGLE);

	float vertex[6][5] {
		{ 0 - 1, 0 - 1, 0, 0, 0 },
		{ 0 - 1, 0 + 1, 0, 0, 1 },
		{ 0 + 1, 0 + 1, 0, 1, 1 },

		{ 0 - 1, 0 - 1, 0, 0, 0 },
		{ 0 + 1, 0 + 1, 0, 1, 1 },
		{ 0 + 1, 0 - 1, 0, 1, 0 },
	};

	fullQuad->setMeshData(vertex, sizeof(vertex), MeshDataLayout::VERT_UV);

	assetManager = new AssetManager();

	physicsThread = new std::thread(&CEngine::physicsLoop, this);

	threadManager = new ThreadManager();

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

	cursorGui = new Graphics::CGui();
	cursorGui->setVisible(true);

	cursor = new Engine::Graphics::Gui::Cursor();
	cursor->setSize(25, 25);
	cursor->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	cursor->setVisible(true);
	
	cursorGui->setCursor(cursor);

}

CEngine::~CEngine() {

	delete cursor;
	delete cursorGui;

	delete groundPlane->shape;
	physEngine->freeStaticObject(groundPlane);

	delete threadManager;

	physicsThread->join();
	delete physicsThread;

	delete physEngine;

	delete assetManager;

	depthWriteShader->release();
	fullQuad->release();
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

void CEngine::setCursor(char* cursorImgPath) {

	cursorTexture = assetManager->loadTexture(cursorImgPath);
	cursor->setTexture(cursorTexture);
}

void CEngine::close() {
	gameWindow->showWindow(false);
}

bool CEngine::setAssetDataFolder(const char* folderPath) {
	bool success = false;
	if (System::folderExists(folderPath)) {
		gAssetDataPath = folderPath;
		success = true;
	}
	return success;
}

const bool CEngine::isRunning() const {
	return running;
}

void CEngine::update(const float dt) {
	Input::Input::GetInput()->reset();

	gameWindow->pollMessages();

	physEngine->update(1.0F/60.0F);

	if (Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);

		renderEngine->updateViewPort(w, h);
		gameWindow->setWindowSize(w, h);

		console->updateSize(w, h);

		windowHeight = h;
		windowWidth = w;

	}

	console->update(dt);

	cursorGui->update(dt);

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
	renderEngine->renderDebugFrame();
}

void CEngine::presentFrame() {
	console->render();
	cursorGui->render();
	gameWindow->swapBuffers();
}

void CEngine::writeDepth(float depthValue, glm::mat4 vpMat, glm::mat4 mdl) {
	depthWriteShader->useShader();
	depthWriteShader->bindData(depthValueLoc, UniformDataType::UNI_FLOAT, &depthValue);
	depthWriteShader->bindData(depthVpMatLoc, UniformDataType::UNI_MATRIX4X4, &vpMat);
	depthWriteShader->bindData(depthMdlMatLoc, UniformDataType::UNI_MATRIX4X4, &mdl);
}

void CEngine::renderFullQuad() {
	fullQuad->bind();
	fullQuad->render();
}

Interfaces::IAssetManager* CEngine::getAssetManager() const {
	return assetManager;
}

PhysicsEngine* CEngine::getPhysEngine() {
	return physEngine;
}

ThreadManager* CEngine::getThreadManager() {
	return threadManager;
}

void CEngine::physicsLoop() {

	System::HighResClock clk;

	float dt = 0.0F;

	while (running) {

		clk.tick();
		dt = clk.seconds();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
}