/// Internal Includes

#include "Engine.hpp"
#include "Input/Input.hpp"
#include "Core/System.hpp"
#include "Graphics/Graphics.hpp"

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

}

CEngine::~CEngine() {

	depthWriteShader->release();
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

void CEngine::writeDepth(float depthValue, glm::mat4 vpMat, glm::mat4 mdl) {
	depthWriteShader->useShader();
	depthWriteShader->bindData(depthValueLoc, UniformDataType::UNI_FLOAT, &depthValue);
	depthWriteShader->bindData(depthVpMatLoc, UniformDataType::UNI_MATRIX4X4, &vpMat);
	depthWriteShader->bindData(depthMdlMatLoc, UniformDataType::UNI_MATRIX4X4, &mdl);
}