#include "Core.hpp"

#include <glm\gtc\matrix_transform.hpp>

#include <thread>

#include <AssetLib\AssetLib.hpp>

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

char* fs_text =
"#version 410 \n"
"layout(location = 0) in vec2 TexCoords;\n"
"layout(location = 0) out vec4 fragmentColor;\n"
"\n"
"layout(location = 1) out vec4 red;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"\n"
"void main() {\n"
"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"	fragmentColor = vec4(1.0, 0.0, 0.0, 1.0) * sampled;\n"
"	red = vec4(1.0, 0.0, 0, 1);\n"
"}  \n";

void Core::init() {
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

	textShader = renderEngine->createShaderObject();
	textShader->init();

	textShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs);
	textShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs_text);

	assert(textShader->buildShader());

	vp_textloc = textShader->getShaderUniform("viewProjMatrix");
	mdl_textloc = textShader->getShaderUniform("worldMat");
	tex_textloc = textShader->getShaderUniform("text");

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
	//void* data = AssetLib::loadWavefrontOBJ("UnitCube.mesh", size);

	planeMesh->setMeshData(f, sizeof(f), VERT_UV);

	//delete []data;


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
	texture->init(4, false);
	unsigned char te[16 * 16];

	for ( size_t i = 0; i < 16; i++ ) {
		for ( size_t j = 0; j < 16; j++ ) {
			te[(i * 16) + j] = (unsigned char)(((float)(i*j) / (16.0f*16.0f)) * 256.0f);
		}
	}

	//texture->setTextureData(16, 16, 1, te);

	game = new Game();
	game->init();

	state = GameState::eGameStage_MainMenu;
	targetState = GameState::eGameStage_MainMenu;

	startWorkerThreads();

	assetMgr = new AssetManager();
	assetMgr->setThreadManager(thrdMgr);

	ma = new ModelAsset();
	ma->init();

	ta = new TextureAsset();
	ta->init();

	Task t;

	mli.type = LoadType::eLoadType_file;
	mli.fileName = "unitcube.mesh";

	mls.asset = ma;
	mls.howToLoad = mli;

	t.type = TaskType::eTaskType_loadAsset;
	t.data = &mls;

	thrdMgr->queueTask(t);

	tli.type = LoadType::eLoadType_file;
	tli.fileName = "texture.bmp";

	tls.asset = ta;
	tls.howToLoad = tli;

	t.type = TaskType::eTaskType_loadAsset;
	t.data = &tls;

	thrdMgr->queueTask(t);

	mainMenu.initMeshes(renderEngine);
	mainMenu.setWindowRes(1280, 720);

	font = renderEngine->createFont();
	font->init("C:/Windows/Fonts/Arial.ttf", 24);
	
	text = new Text();
	text->init(renderEngine);
	text->setFont(font);

	text->setText("Asdf", 4, 0, 0, 1.0);

}

void Core::release() {
	text->release();
	font->release();

	mainMenu.releaseMeshes();
	ma->release();
	ta->release();

	stopWorkerThreads();

	disp.setWindow(nullptr);
	disp.setRenderEngine(nullptr);

	delete assetMgr;
	delete thrdMgr;
	delete game;
	delete console;

	fbo->release();

	textShader->release();
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
	thrdMgr->startThreads(8);
}

void Core::stopWorkerThreads() {
	if ( thrdMgr )
		thrdMgr->stopThreads();
}

void Core::setFPS(int _fps) {
	fps = _fps;
}

void Core::update(float dt) {
	// poll messages and update camera
	window->pollMessages();
	camInput.update(dt);

	// window size change event
	if ( input->sizeChange ) {
		int w = 0;
		int h = 0;
		input->getWindowSize(w, h);
		mainMenu.setWindowRes(w, h);

		Resolution res = disp.getResolution();
		renderEngine->updateViewPort(res.width, res.height);
		fbo->resize(res.width, res.height);
		fbo->setWindowSize(w, h);
		if ( w != 0 && h != 0 ) {
			*(glm::mat4*)(camera->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(res.width), float(res.height), 0.0001f, 100.0f);
		}
		//console->print("Resize\n");
	}

	running = window->isVisible();

	switch ( state ) {
		case GameState::eGameStage_MainMenu:
			updateMainMenu(dt);
			break;
		case GameState::eGameState_EditorMode:
			updateEditor(dt);
			break;
		case GameState::eGameState_PlayMode:
			updateGame(dt);
			break;
		case GameState::eGameState_loading:
			break;
		case GameState::eGameState_Undefined:
			assert(0 && "Undefined State");
			break;
		default:
			break;
	}
	// update gameState
	game->update(dt);

	if ( input->consoleKeyWasPressed() ) {
		input->toggleConsole();
		console->setVisible(input->consoleIsActive());

	}

	glm::mat4 vp = *(glm::mat4*)camera->getPerspectiveMatrix() * *(glm::mat4*)camera->getViewMatrix();

	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	shader->useShader();
	shader->bindData(vpLoc, UniformDataType::UNI_MATRIX4X4, &vp);
	shader->bindData(mdlLoc, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

	// temp asset transfer
	if ( ma->getAssetState() == AssetState::eAssetState_loaded ) {
		planeMesh->setMeshData(ma->getDataPtr(), ma->getDataSize(), MeshDataLayout::VERT_UV);
		ma->setAssetState(AssetState::eAssetState_loadedGPU);
	}
	if ( ta->getAssetState() == AssetState::eAssetState_loaded ) {
		int w, h;
		ta->getTextureSize(w, h);
		texture->setTextureData(w, h, 4, ta->getDataPtr());
		ta->setAssetState(AssetState::eAssetState_loadedGPU);
	}

	// reset input states, clear for next frame
	input->reset();
}

void Core::render() {
	fbo->bind();
	renderEngine->setBlending(false);
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
	
	// render GUI

	shader->bindData(vpLoc, UniformDataType::UNI_MATRIX4X4, camera->getOrthoMatrix());
	shader->bindData(mdlLoc, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

	mainMenu.render();

	renderEngine->setDepthTest(false);
	renderEngine->setBlending(true);

	textShader->useShader();
	textShader->bindData(vp_textloc, UniformDataType::UNI_MATRIX4X4, camera->getOrthoMatrix());
	textShader->bindData(mdl_textloc, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

	text->render(textShader, tex_textloc);

	// resolve
	fbo->resolveToScreen();
	hadReset = renderEngine->getGraphicsReset();
	if ( hadReset ) return;

	// swap
	window->swapBuffers();
}

DisplaySettings * Core::getDisplaySettings() {
	return &disp;
}

void Core::updateMainMenu(float dt) {

	int x = 0;
	int y = 0;
	input->getMousePos(x, y);
	
	//printf("Mouse pos (%d, %d)\n", x, y);

	if ( mainMenu.isNewGamePressed() ) {
		mainMenu.setVisible(false);
		text->setText("New Game", 8, 0, 0, 1.0);
		enterNewGame();
	} else if ( mainMenu.isLoadGamePressed() ) {
		mainMenu.setVisible(false);
		text->setText("Load Game", 9, 0, 0, 1.0);
		loadGame();
	} else if ( mainMenu.isContinueGamePressed() ) {
		text->setText("Continue", 8, 0, 0, 1.0);
		mainMenu.setVisible(false);
	} else if ( mainMenu.isEditorPressed() ) {
		mainMenu.setVisible(false);
		text->setText("Editor", 6, 0, 0, 1.0);
		enterEditor();
	} else if ( mainMenu.isOptionsPressed() ) {
		text->setText("Options", 7, 0, 0, 1.0);
		mainMenu.setVisible(false);
	} else if ( mainMenu.isQuitPressed() ) {
		window->showWindow(false);
	}
}

void Core::updateEditor(float dt) {

}

void Core::updateGame(float dt) {
	game->update(dt);
}

void Core::enterNewGame() {
	state = GameState::eGameState_loading;
	targetState = GameState::eGameState_PlayMode;
}

void Core::loadGame() {
	state = GameState::eGameState_loading;
	targetState = GameState::eGameState_PlayMode;
}

void Core::enterEditor() {
	state = GameState::eGameState_loading;
	targetState = GameState::eGameState_EditorMode;
}
