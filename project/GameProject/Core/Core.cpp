#include "Core.hpp"
#include "System\Sys.hpp"

#include "Script\OcParser.hpp"

#include <AssetLib\AssetLib.hpp>

#include <thread>

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

// temporary
// move elsewhere

struct Header {
	char tag[4];
	uint16_t major;
	uint16_t minor;
};

struct BoolFlags
{
	uint32_t meshCount;

	bool useVNormals;
	bool useVColors;
	bool useVUV;
	bool padding;
	uint32_t vertCount;
	uint32_t triangleCount;
};

struct Triangle {
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;
};

struct Vertex5
{
	glm::vec3 pos;
	glm::vec2 uv;
};

void* createVertUVData(void* meshData, uint32_t &size) {

	Header* h = (Header*)meshData;
	BoolFlags* bf = (BoolFlags*) ((char*)meshData + sizeof(Header) );

	glm::vec3* vertices = (glm::vec3*)((char*)meshData + sizeof(Header) + sizeof(BoolFlags));
	Triangle* triangles = (Triangle*)((char*)meshData + sizeof(Header) + sizeof(BoolFlags) + (sizeof(float) * 3 * bf->vertCount));

	std::vector<Vertex5> verts;

	for ( size_t i = 0; i < bf->triangleCount; i++ ) {
		Triangle t = triangles[i];
		
		Vertex5 v1  = { vertices[t.v1], glm::vec2(0.0f, 0.0f) };
		Vertex5 v2 = { vertices[t.v2], glm::vec2(0.0f, 0.0f) };
		Vertex5 v3 = { vertices[t.v3], glm::vec2(0.0f, 0.0f) };
		
		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
	}

	size = (uint32_t)verts.size() * sizeof(Vertex5);

	Vertex5* v = new Vertex5[verts.size()];
	memcpy(v, verts.data(), size);

	return v;
}

// end temporary
// end move elsewhere

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

	// temporary

	shaderObj = renderEngine->createShaderObject();
	
	std::string vs = readShader("data/shaders/default.vs.glsl");
	std::string gs = readShader("data/shaders/default.gs.glsl");
	std::string fs = readShader("data/shaders/default.fs.glsl");

	shaderObj->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	shaderObj->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	shaderObj->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if(!shaderObj->buildShader()) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	vp = shaderObj->getShaderUniform("viewProjMatrix");
	mdl = shaderObj->getShaderUniform("worldMat");

	// end temporary

	performParserTests();
	//parseOcFile("Data/Scripts/test.ocs");
}

void Core::release() {

	shaderObj->release();

	stopWorkerThreads();

	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	delete thrdMgr;

	delete console;

	renderEngine->release();

	renderEngineLib.unloadLibrary();
	mem.release();
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
