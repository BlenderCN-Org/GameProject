#include "Core.hpp"
#include "CoreGlobals.hpp"

#include "System\Sys.hpp"

#include "Script\OcParser.hpp"

#include <AssetLib\AssetLib.hpp>

#include <thread>

int Core::width = 0;
int Core::heigth = 0;

std::string readShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Could not open file %s", filePath);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
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
	if (renderEngineLib.loadLibrary("RenderEngine.dll\0"))
		printf("Loaded RenderEngine.dll\n");
	else {
		printf("Failed to load Renderer\n");
		throw;
	}

	if (editorLib.loadLibrary("Editor_Wrapper.dll\0"))
		printf("Loaded Editor_Wrapper.dll\n");
	else {
		printf("Failed to load Editor\n");
		throw;
	}

	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	CreateEditorProc eProc = (CreateEditorProc)editorLib.getProcAddress("CreateEditor");
	editor = eProc();

	extHandler = new ExtensionHandler();

	extHandler->loadExtensions(editor);

	disp.setResolution(1280, 720);
	width = 1280;
	heigth = 720;
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

	pixelBuffers[0] = renderEngine->createPixelBuffer();
	pixelBuffers[1] = renderEngine->createPixelBuffer();

	pixelBuffers[0]->init();
	pixelBuffers[1]->init();

	pixelBuffers[0]->resize(width, heigth);
	pixelBuffers[1]->resize(width, heigth);

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

	if (!shaderObj->buildShader()) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	if (!textShaderObj->buildShader()) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	vp = shaderObj->getShaderUniform("viewProjMatrix");
	mdl = shaderObj->getShaderUniform("worldMat");

	text = new Text();
	text->init(renderEngine);
	text->setFont(AssetManager::getAssetManager()->getBasicFont());

	g_core = this;
	g_assetManager = assetManager;
	g_threadManager = thrdMgr;
	g_memoryManager = &mem;
	g_renderEngine = renderEngine;

	// end temporary
	performParserTests();
	//parseOcFile("Data/Scripts/test.ocs");
}

void Core::freeResources() {
	// @Temporary

	pixelBuffers[0]->release();
	pixelBuffers[1]->release();

	extHandler->unloadExtension(editor);
	delete extHandler;
	editor->releaseEditor();
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

	g_core = nullptr;
	g_assetManager = nullptr;
	g_threadManager = nullptr;
	g_memoryManager = nullptr;
	g_renderEngine = nullptr;
}

bool Core::isRunning() {
	return running;
}

bool Core::hadGraphicsReset() const {
	if (console->reset) {
		console->reset = false;
		return true;
	}
	return renderEngine->getGraphicsReset();
}

bool Core::editorAvaible() const {
	return (nullptr != editor);
}

bool Core::isInEditor() const {
	return editor->getStatus() == EditorStatus::RUNNING;
}

void Core::startEditor() {
	if (editorAvaible() && editor->initializeEditor()) {

		//input->setupCallbacks(editor->getEditorWindow());
		//
		//disp.setFullscreenMode(FullscreenMode::WINDOWED_BORDERLESS);
		//disp.apply();
		//
		//window->showWindow(false);
		editor->startEditor();
		//Task t;
		//t.type = TaskType::eTaskType_execute;
		//ExecuteTask* task = new ExecuteTask;
		//task->ptr = editor;
		//task->call = [](void* x) { ((IEditor*)x)->startEditor(); };
		//t.data = task;
		//thrdMgr->queueTask(t);
	}
}

void Core::editorDetachGameWindow() {}

void Core::stopEditor() {
	editor->stopEditor();
}

void Core::startWorkerThreads() {
	thrdMgr->startThreads(getLogicalProcessorCount());
}

void Core::stopWorkerThreads() {
	if (thrdMgr)
		thrdMgr->stopThreads();
}

void Core::setFPS(int _fps) {
	fps = _fps;
}

void Core::update(float dt) {
	mem.getFrameAllocator()->reset();
	mem.getTimeAllocator()->tick(dt);
	// reset input states, clear for next frame
	input->reset();
	window->pollMessages();
	EditorStatus status = editor->getStatus();
	if (editor && status == EditorStatus::RUNNING || status == EditorStatus::HIDDEN) {
		editor->update();
	}
	// poll messages and update camera
	running = window->isVisible() || status == EditorStatus::RUNNING || status == EditorStatus::HIDDEN;

	// we are changing status
	if (currentEditorStatus != status) {

		switch (status) {
			case STARTING:
				break;
			case RUNNING:
				window->showWindow(false);
				input->setupCallbacks(editor->getEditorWindow());
				break;
			case HIDDEN:
				window->showWindow(true);
				input->setupCallbacks(window);
				break;
			case STOPPING:
				break;
			case STOPPED:
				break;
			default:
				break;
		}

		currentEditorStatus = status;
	}

	if (window->isVisible() == false && status == EditorStatus::HIDDEN) {
		stopEditor();
	}

	// window size change event
	if (input->sizeChange) {
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);

		renderEngine->updateViewPort(w, h);
		pixelBuffers[0]->resize(w, h);
		pixelBuffers[1]->resize(w, h);
		//Resolution res = disp.getResolution();
		//renderEngine->updateViewPort(res.width, res.height);
		width = w;
		heigth = h;
	}

	if (input->consoleKeyWasPressed()) {
		input->toggleConsole();
		console->setVisible(input->consoleIsActive());
	}

	hadReset = renderEngine->getGraphicsReset();
	if (hadReset) return;
}

void Core::render(glm::mat4 viewMat) {

	renderEngine->renderDebugFrame();
	hadReset = renderEngine->getGraphicsReset();
	if (hadReset) return;

	/* PBO readback */
	pboIndex = (pboIndex + 1) % 2;
	pboNextIndex = (pboIndex + 1) % 2;

	pixelBuffers[pboIndex]->read(width, heigth);

	void* data = pixelBuffers[pboNextIndex]->map();
	uint32_t w, h;
	pixelBuffers[pboNextIndex]->getSize(w, h);

	if (data && isInEditor()) {
		editor->postPixels(width, heigth, data);
	}
	pixelBuffers[pboNextIndex]->unMap();

	/* PBO readback */

}

void Core::renderConsole() {

	if (input->consoleIsActive()) {
		textShaderObj->useShader();
		renderEngine->setBlending(true);
		std::string consoleText = console->getText();
		text->setText((char*)consoleText.c_str(), consoleText.size(), 10, 1080 - 50, 1.0f);
		text->render(textShaderObj, 0);

		std::string consoleHistory = console->getHistory();

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

Console * Core::getConsole() {
	return console;
}