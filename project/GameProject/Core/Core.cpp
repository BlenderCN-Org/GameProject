#include "Core.hpp"
#include "System\Sys.hpp"

#include "Script\OcParser.hpp"

#include <AssetLib\AssetLib.hpp>

#include <thread>

int Core::width = 0;
int Core::heigth = 0;

std::string readShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if ( !fileStream.is_open() ) {
		printf("Could not open file %s", filePath);
		return "";
	}

	std::string line = "";
	while ( !fileStream.eof() ) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

Core::~Core() {
	freeResources();
}

void Core::init() {
	initSys();
	mem.init();
	thrdMgr = nullptr;

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
	width = 720;
	heigth = 1080;
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

	thrdMgr = ThreadManager::getThreadManager();

	startWorkerThreads();

	assetManager = AssetManager::getAssetManager();
	assetManager->init(renderEngine);

	// temporary
	shaderObj = renderEngine->createShaderObject();
	textShaderObj = renderEngine->createShaderObject();

	std::string vs = readShader("data/shaders/default.vs.glsl");
	std::string gs = readShader("data/shaders/default.gs.glsl");
	std::string fs = readShader("data/shaders/default.fs.glsl");
	std::string tfs = readShader("data/shaders/text.fs.glsl");

	shaderObj->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	shaderObj->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	shaderObj->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	textShaderObj->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	textShaderObj->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)tfs.c_str());

	if ( !shaderObj->buildShader() ) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	if ( !textShaderObj->buildShader() ) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	vp = shaderObj->getShaderUniform("viewProjMatrix");
	mdl = shaderObj->getShaderUniform("worldMat");

	text = new Text();
	text->init(renderEngine);
	text->setFont(AssetManager::getAssetManager()->getSmallFont());

	// end temporary

	console->print("Stuff :p");

	performParserTests();
	//parseOcFile("Data/Scripts/test.ocs");
}

void Core::freeResources() {
	// @Temporary
	
	delete text;

	textShaderObj->release();
	shaderObj->release();

	assetManager->release();

	stopWorkerThreads();
	thrdMgr->release();

	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	delete console;

	renderEngine->release();

	renderEngineLib.unloadLibrary();
	mem.release();
	input->release();

	deinitSys();
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
	mem.getFrameAllocator()->reset();
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

void Core::render(glm::mat4 viewMat) {
	renderEngine->renderDebugFrame();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;
}

void Core::renderConsole() {

	if ( input->consoleIsActive() ) {
		textShaderObj->useShader();
		renderEngine->setBlending(true);
		std::string consoleText = console->getText();
		text->setText((char*)consoleText.c_str(), consoleText.size(), 10, 1080 - 50, 1.0f);
 		text->render(textShaderObj, 0);

		std::string consoleHistory = console->getHistory();

		IFont* fnt = assetManager->getSmallFont();

		text->setText((char*)consoleHistory.c_str(), consoleHistory.size(), 10, 1080 - 750, 1.0f);
		text->render(textShaderObj, 0);

		renderEngine->setBlending(false);
	}

}

void Core::swap() {
	// swap
	window->swapBuffers();
}

DisplaySettings * Core::getDisplaySettings() {
	return &disp;
}

MemoryManager* Core::getMemoryManager() {
	return &mem;
}

IRenderEngine * Core::getRenderEngine() {
	return renderEngine;
}

IShaderObject * Core::getShaderObject() {
	return shaderObj;
}

IShaderObject * Core::getTextShaderObject() {
	return textShaderObj;
}