#include "Core.hpp"

#include <glm\gtc\matrix_transform.hpp>

#include <thread>

#include "Loading\OBJLoader.hpp"

#include <GL\GL.h>

char* vs =
"#version 410\n"
"\n"
"layout(location = 0) in vec3 vertexPos;\n"
"layout(location = 1) in vec2 vertexUv;\n"
"\n"
"layout(location = 0) out vec2 fragUv;\n"
"\n"
"uniform mat4 viewProjMatrix;\n"
"uniform mat4 worldMat;\n"
"\n"
"void main()\n"
"{\n"
"	fragUv = vertexUv;\n"
"	gl_Position = viewProjMatrix * vec4((vec4(vertexPos, 1.0f) * worldMat).xyz, 1.0f);\n"
"}\n";

char* gs =
"#version 410\n"
"layout(triangles) in;\n"
"layout(triangle_strip, max_vertices = 3) out;\n"
"\n"
"layout(location = 0) in vec2 fragUv[];\n"
"\n"
"layout(location = 0) out vec2 UV;\n"
"\n"
"void main() {\n"
"UV = fragUv[0];\n"
"gl_Position = gl_in[0].gl_Position;\n"
"EmitVertex();\n"
"UV = fragUv[1];\n"
"gl_Position = gl_in[1].gl_Position;\n"
"EmitVertex();\n"
"UV = fragUv[2];\n"
"gl_Position = gl_in[2].gl_Position;\n"
"EmitVertex();\n"
"EndPrimitive();\n"
"}\n";


char* fs =
"#version 410\n"
"\n"
"layout(location = 0) in vec2 fragUv;\n"
"\n"
"layout(location = 0) out vec4 fragmentColor;\n"
"\n"
"layout(location = 1) out vec4 red;\n"
"uniform sampler2D textureIN;\n"
"void main()\n"
"{\n"
"	vec4 color = texture(textureIN, vec2(fragUv.x, fragUv.y));\n"
//"	color.r = 1.0;"
//"	color.g = 1.0;"
//"	color.b = 1.0;"
//"	color.a = 1.0;"
"	fragmentColor = color; //vec4(fragUv.x, fragUv.y, 0, 1);\n"
"	red = vec4(1.0, 0.0, 0, 1);\n"
"}\n";

void Core::init()
{
	running = true;
	hadReset = false;
	if ( renderEngineLib.loadLibrary("RenderEngine.dll\0") )
		printf("Loaded\n");
	else
	{
		printf("Failed to load\n");
		throw;
	}
	CreateRenderEngineProc rProc = (CreateRenderEngineProc)renderEngineLib.getProcAddress("CreateRenderEngine");

	disp.setResolution(1280, 720);
	disp.setVsyncMode(VSyncMode::VSYNC_ON);
	disp.setFramerateLock(FramerateLock::FRAME_LOCK_NONE);
	disp.setFullscreenMode(FullscreenMode::WINDOWED);
	
	//renderEngine = CreateRenderEngine();

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

	camera = renderEngine->createCamera();

	shader = renderEngine->createShaderObject();
	shader->init();

	shader->setShaderCode(ShaderStages::VERTEX_STAGE, vs);
	shader->setShaderCode(ShaderStages::GEOMETRY_STAGE, gs);
	shader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs);

	assert(shader->buildShader());

	vpLoc = shader->getShaderUniform("viewProjMatrix");
	mdlLoc = shader->getShaderUniform("worldMat");
	texLoc = shader->getShaderUniform("textureIN");

	camInput.init((glm::mat4*)camera->getViewMatrix());

	*(glm::mat4*)(camera->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);
	*(glm::mat4*)(camera->getOrthoMatrix()) = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);

	input->attachConsole(console);

	planeMesh = renderEngine->createMesh();
	planeMesh->init(MeshPrimitiveType::TRIANGLE);

	float f[] = { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f };

	unsigned int size = 0;
	void* data = ObjLoader::load("UnitCube.mesh", size);

	planeMesh->setMeshData(f, sizeof(f), VERT_UV);

	delete []data;


	fbo = renderEngine->createFrameBuffer();

	FrameBufferCreateInfo fboCreate = {};

	fboCreate.width = 1280;
	fboCreate.height = 720;
	fboCreate.nrColorBuffers = 2;
	fboCreate.useDepth = true;
	fboCreate.useRenderbuffer = true;
	fboCreate.useMultisample = false;
	fboCreate.useStencil = true;
	fboCreate.mutlisample = 0;

	fbo->init(&fboCreate);


	texture = renderEngine->createTexture();
	texture->init(1, false);
	unsigned char te[16 * 16];
	
	for ( size_t i = 0; i < 16; i++ )
	{
		for ( size_t j = 0; j < 16; j++ )
		{
			te[(i*16) + j] = (unsigned char) (((float)(i*j)/(16.0f*16.0f)) * 256.0f);
		}
	}
	
	texture->setTextureData(16, 16, 1, te);

	game = new Game();
	game->init();

	startWorkerThreads();

	ma = new ModelAsset();
	ma->init();

	Task t;

	li.type = LoadType::eLoadType_file;
	li.fileName = "Unitcube.mesh";

	ls.asset = ma;
	ls.howToLoad = li;

	t.type = TaskType::eTaskType_loadAsset;
	t.data = &ls;

	thrdMgr.queueTask(t);
}

void Core::release()
{
	ma->release();

	stopWorkerThreads();

	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	delete game;
	delete console;

	fbo->release();

	shader->release();
	camera->release();
	
	planeMesh->release();

	texture->release();

	renderEngine->release();

	renderEngineLib.unloadLibrary();
	delete this;
}

bool Core::isRunning() {
	return running;
}

bool Core::hadGraphicsReset() const
{
	if ( console->reset )
	{
		console->reset = false;
		return true;
	}
	return renderEngine->getGraphicsReset();
}

void Core::startWorkerThreads() {
	thrdMgr.startThreads(8);
}

void Core::stopWorkerThreads() {
	thrdMgr.stopThreads();
}

void Core::setFPS(int _fps)
{
	fps = _fps;
}

void Core::update(float dt)
{
	window->pollMessages();
	camInput.update(dt);

	running = window->isVisible();

	game->update(dt);

	if (input->consoleKeyWasPressed())
	{
		input->toggleConsole();
		console->setVisible(input->consoleIsActive());
		
	}

	glm::mat4 vp = *(glm::mat4*)camera->getPerspectiveMatrix() * *(glm::mat4*)camera->getViewMatrix();

	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	shader->useShader();
	shader->bindData(vpLoc, UniformDataType::UNI_MATRIX4X4, &vp);
	shader->bindData(mdlLoc, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

	if (input->sizeChange)
	{
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);

		Resolution res = disp.getResolution();
		renderEngine->updateViewPort(res.width, res.height);
		fbo->resize(res.width, res.height);
		fbo->setWindowSize(w, h);
		if ( w != 0 && h != 0 )
		{
			*(glm::mat4*)(camera->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(res.width), float(res.height), 0.0001f, 100.0f);
		}
		//console->print("Resize\n");
	}

	if ( ma->getAssetState() == AssetState::eAssetState_loaded ) 		{
		planeMesh->setMeshData(ma->getDataPtr(), ma->getDataSize(), MeshDataLayout::VERT_UV);
	}

	//input->print();
	input->reset();
}

void Core::render()
{
	fbo->bind();
	renderEngine->setDepthTest(true);
	renderEngine->setStencilTest(true);
	renderEngine->stencilMask(0xFF);
	renderEngine->renderDebugFrame();
	renderEngine->stencilOp(GL_INCR, GL_INCR, GL_INCR);
	renderEngine->stencilFunc(GL_ALWAYS, 1, 0xFF);
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	planeMesh->bind();

	texture->bind();
	
	int id = 0;
	shader->bindData(texLoc, UniformDataType::UNI_INT, &id);
	
	planeMesh->render();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	fbo->resolveToScreen();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;
		
	window->swapBuffers();
}

DisplaySettings * Core::getDisplaySettings()
{
	return &disp;
}
