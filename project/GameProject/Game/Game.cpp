
/// Internal Includess
#include "Game.hpp"
#include "../Engine/Graphics/Graphics.hpp"
#include "../Engine/Core/System.hpp"

/// External Includes
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// Std Includes
#include <fstream>
#include <string>

// @temporary
inline std::string readShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		//Debug::DebugOutput("Could not open file %s", filePath);
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

TemporaryStorage ts(16 * KB);

// @temporary end

Game::Game(CEngine* _engine)
	: gameGui(nullptr)
	, engine(_engine)
	, mesh(nullptr)
	, dtOneSec(0.0F)
	, fps(0)
	, fpsCounter(0)
	, batchTmp(nullptr) {

	currentState = GameState::MAIN_MENU;
	lastState = GameState::MAIN_MENU;

	player = new Player();

	camInput.init((glm::mat4*)camera.getViewMatrix());
	camInput.setCam(glm::vec3(5, 1, 0), glm::vec3(-1, 0, 0));

	player->setCamera(&camInput);

	*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0F), 1280.0F, 720.0F, 0.1F, 100.0F);

	mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/HollowCylinder.mesh");
	aabbTest = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/V2/HalfCube.mesh");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/newFmat.mesh");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/untitled.dae");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadDea("Data/Meshes/untitled.dae");

	//mesh = new Engine::Graphics::Mesh::CMesh();
	//mesh->loadMesh("Data/Meshes/Test_exteriorScene_vColor.mesh");
	//mesh->loadMesh("Data/Meshes/newFmat.skel");
	gameGui = new Engine::Graphics::CGui();
	gameGui->setVisible(true);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	metrixPanel = new Engine::Graphics::Gui::Panel();
	metrixPanel->setPosition(-10, 10);
	metrixPanel->setSize(300, 150);
	metrixPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	metrixPanel->setVisible(true);
	metrixPanel->setTexture(panelTexture);

	infoLabel = new Engine::Graphics::Gui::Label();
	infoLabel->setSize(300, 150);
	infoLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	infoLabel->setVisible(true);
	infoLabel->setText("test");

	gameGui->addGuiItem(metrixPanel);
	metrixPanel->addGuiItem(infoLabel);

	shader = gRenderEngine->createShaderObject();
	shader->init();

	std::string vs = readShader("data/shaders/default.vs.glsl");
	std::string gs = readShader("data/shaders/default.gs.glsl");
	std::string fs = readShader("data/shaders/default.fs.glsl");

	shader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	shader->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	shader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!shader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	vpLocation = shader->getShaderUniform("viewProjMatrix");
	matLocation = shader->getShaderUniform("worldMat");
	selectedLoc = shader->getShaderUniform("selectedColor");

	gRenderEngine->setClearColor(0.300290F, 0.469524F, 0.581466F, 1.000000F);

	r = 10;
	a = 30;

	mirror = new Engine::Graphics::Mesh::MirrorMesh();
	mirror->setSize(glm::vec2(5, 2));

	FrameBufferCreateInfo fbci {};

	fbci.height = 720;
	fbci.width = 1280;
	fbci.mutlisample = 8;
	fbci.nrColorBuffers = 3;
	fbci.useDepth = true;
	fbci.useMultisample = true;
	fbci.useRenderbuffer = false;
	fbci.useStencil = true;

	gBuffer = gRenderEngine->createFrameBuffer();
	gBuffer->init(&fbci);

	gBufferShader = gRenderEngine->createShaderObject();
	gBufferShader->init();

	vs = readShader("data/shaders/gbuffer.vs.glsl");
	gs = readShader("data/shaders/gbuffer.gs.glsl");
	fs = readShader("data/shaders/gbuffer.fs.glsl");

	gBufferShader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	gBufferShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	gBufferShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!gBufferShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	vpLocationGBuff = gBufferShader->getShaderUniform("viewProjMatrix");
	matLocationGBuff = gBufferShader->getShaderUniform("worldMat");
	selectedLocGBuff = gBufferShader->getShaderUniform("selectedColor");
	refMatLocationGBuff = gBufferShader->getShaderUniform("reflectMat");
	clipPlane = gBufferShader->getShaderUniform("clipPlane");
	skinArray = gBufferShader->getShaderUniform("skinMatrices");

	camPath.init(&camInput);
	camPath.followPaths(false);

	sky = new Sky();

	gBufferBlit = gRenderEngine->createShaderObject();
	gBufferBlit->init();

	vs = readShader("data/shaders/BlitShader.vs.glsl");
	fs = readShader("data/shaders/BlitShader.fs.glsl");

	char* cvs = (char*)vs.c_str();
	char* cfs = (char*)fs.c_str();

	gBufferBlit->setShaderCode(ShaderStages::VERTEX_STAGE, cvs);
	gBufferBlit->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	gBufferBlit->setShaderCode(ShaderStages::FRAGMENT_STAGE, cfs);

	if (!gBufferBlit->buildShader()) {
		assert(0 && "shader failed to build");
	}

	blitTexDiff = gBufferBlit->getShaderUniform("textureDiff");
	blitTexNorm = gBufferBlit->getShaderUniform("textureNorm");
	blitTexWPos = gBufferBlit->getShaderUniform("textureWPos");
	blitTextShadow = gBufferBlit->getShaderUniform("textureShadow");
	blitDepthMvp = gBufferBlit->getShaderUniform("depthMVP");
	blitEyePos = gBufferBlit->getShaderUniform("eyePos");

	shadowMap = gRenderEngine->createFrameBuffer();

	fbci.width = 1024 * 4;
	fbci.height = 1024 * 4;

	fbci.useDepth = true;
	fbci.nrColorBuffers = 0;
	fbci.mutlisample = 16;
	fbci.useStencil = false;
	fbci.useMultisample = true;

	shadowMap->init(&fbci);

	shadowShader = gRenderEngine->createShaderObject();
	shadowShader->init();

	vs = readShader("data/shaders/shadow.vs.glsl");
	fs = readShader("data/shaders/shadow.fs.glsl");

	shadowShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	shadowShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	shadowShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!shadowShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	shadowMVP = shadowShader->getShaderUniform("depthMVP");

	menu = new MainMenu();

	editor = new Engine::RefObject<Editor>(5.0F);

	mapLoader.openMap("E:/GameProjectAssets/Data/GameMap.map");

	LoadedData data = mapLoader.loadDataEntry(1);

	map = new Map(data, mapLoader);

	free(data.data);

	asteroidShip = new Ship();

	asteroidShader = gRenderEngine->createShaderObject();
	asteroidShader->init();

	vs = readShader("data/shaders/asteroids/ast.vs.glsl");
	fs = readShader("data/shaders/asteroids/ast.fs.glsl");

	asteroidShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	asteroidShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	asteroidShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!asteroidShader->buildShader()) {
		assert(0 && "asteroidShader failed to build");
	}

	asteroidVP = asteroidShader->getShaderUniform("viewProjMatrix");
	asteroidMdl = asteroidShader->getShaderUniform("worldMat");
	asteroidColor = asteroidShader->getShaderUniform("selectedColor");

	bulletMesh = gRenderEngine->createMesh();
	bulletMesh->init(MeshPrimitiveType::TRIANGLE);

	float points[] = {
		+2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		+2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,
		-2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,

		-2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		+2.0F * 1.0F, 0.0F, +2.0F * 1.0F, 0.0F, 0.0F,
		-2.0F * 1.0F, 0.0F, -2.0F * 1.0F, 0.0F, 0.0F,

	};

	bulletMesh->setMeshData(&points, sizeof(points), MeshDataLayout::VERT_UV);

	meshRotation = 0.0F;
}

Game::~Game() {

	auto bit = bullets.begin();
	auto beit = bullets.end();

	while (bit != beit) {
		Bullet* b = *bit;

		delete b;
		bit = bullets.erase(bit);
		beit = bullets.end();
	}

	auto ait = asteroids.begin();
	auto aeit = asteroids.end();

	while (ait != aeit) {
		Asteroid* a = *ait;

		delete a;
		ait = asteroids.erase(ait);
		aeit = asteroids.end();
	}

	bulletMesh->release();
	asteroidShader->release();
	delete asteroidShip;

	delete map;

	delete editor;

	delete menu;

	delete aabbTest;

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	delete mirror;

	shader->release();
	gBuffer->release();
	gBufferShader->release();
	gBufferBlit->release();
	shadowMap->release();
	shadowShader->release();

	if (gameGui) {
		delete gameGui;
		gameGui = nullptr;
	}
	delete metrixPanel;
	delete infoLabel;
	delete panelTexture;
	delete sky;

	delete player;
}

void Game::update(float dt, uint64_t clocks) {

	size_t tUsed = ts.getUsed();

	ts.clear();

	batchTmp = ts.allocate<RenderBatch>(1, 1000, &ts);

	frameObjects.clear();

	engine->update(dt);
	editor->refUpdate(dt);
	updateFps(dt);

	gameGui->update(dt);

	if (Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);
		gBuffer->resize(w, h);
		gBuffer->setWindowSize(w, h);
	}

	//ts.

	bool clear = true;

	ActiveFrameBufferCommand* fbCmd = ts.allocate<ActiveFrameBufferCommand>(1, gBuffer, clear);
	batchTmp->addCommand(fbCmd);

	//ActiveShaderCommand* asc = ts.allocate<ActiveShaderCommand>(1, gBufferShader);
	//batchTmp->addCommand(asc);

	switch (currentState) {
	case GameState::MAIN_MENU:
		updateMenu(dt);
		break;
	case GameState::PLAY:
		updatePlay(dt);
		break;
	case GameState::EDIT:
		updateEdit(dt);
		break;
	case GameState::PAUSE:
		updatePaused(dt);
		break;
	case GameState::PLAY_ASTEROIDS:
		updateAsteroids(dt);
		break;
	default:
		break;
	}

	vpMat = camera.viewProjection();

	camPath.update(dt);

	sky->update(dt);


	float x = r * glm::sin(a);
	float z = r * glm::cos(a);

	glm::vec3 posx = glm::vec3(x, 2, z);

	mirror->setMirrorPosition(posx);
	mirror->setMirrorNormal(-glm::normalize(posx - glm::vec3(0, 2, 0)));
	mirror->setSize(glm::vec2(10, 5));


	std::string str = "FPS: " + std::to_string(fps) + "\n";
	str += "Dt: " + std::to_string(dt) + "\n";

	glm::vec3 pos = camera.getPos();
	str += "Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "\n";

	glm::vec3 dir = camInput.direction();
	str += "Dir: (" + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z) + "\n";

	str += "TempUsed: " + std::to_string(tUsed) + " Bytes\n";

	str += "Clocks: " + std::to_string(clocks) + "\n";

	str += "RotationSpeed: " + std::to_string(rotSpeed) + "\n";

	str += "Rotation: " + std::to_string(meshRotation * toDEGREE) + " (" + std::to_string((meshRotation * toDEGREE) / 360.0F) + ")\n";
	str += "RotTime: " + std::to_string(rotTime) + "\n";

	Input::InputEvent ie = Input::Input::GetInput()->lastPressed;

	str += "Last Key: " + std::string(ie.mouse ? "mouse " : "key ") + std::to_string(ie.code) + "\n";

	//str += "SkyTime: " + std::to_string(skyTime) + "\n";

	infoLabel->setText(str.c_str());
}

void Game::render() {

	if (currentState != GameState::PLAY_ASTEROIDS) {

		//renderShadowMap();
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);
		gRenderEngine->updateViewPort(w, h);

		//gBuffer->bind();
		//gBuffer->clear();

		renderSky();

		batchTmp->executeBatch();

		//renderSky();

		renderScene();

		//gBuffer->resolveToScreen(index);
		//gBuffer->resolveAllToScreen();

		int tex = 0;
		gBufferBlit->useShader();
		gBufferBlit->bindData(blitTexDiff, UniformDataType::UNI_INT, &tex);
		tex = 1;
		gBufferBlit->bindData(blitTexNorm, UniformDataType::UNI_INT, &tex);
		tex = 2;
		gBufferBlit->bindData(blitTexWPos, UniformDataType::UNI_INT, &tex);
		tex = 3;
		gBufferBlit->bindData(blitTextShadow, UniformDataType::UNI_INT, &tex);

		glm::vec3 lightDir = glm::vec3(1, 0, 0);//sunMoonDir;

		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -50, 10);
		glm::mat4 depthViewMatrix = glm::lookAt(-lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

		gBufferBlit->bindData(blitDepthMvp, UniformDataType::UNI_MATRIX4X4, &depthBiasMVP);

		gBufferBlit->bindData(blitEyePos, UniformDataType::UNI_FLOAT3, &camera.getPos());

		//gRenderEngine->activeTexture(0);
		//gBuffer->bindAttachment(0);
		//gRenderEngine->activeTexture(1);
		//gBuffer->bindAttachment(1);
		//gRenderEngine->activeTexture(2);
		//gBuffer->bindAttachment(2);
		//gRenderEngine->activeTexture(3);
		//shadowMap->bindAttachment(1);
		//
		//gRenderEngine->bindDefaultFrameBuffer();
		//engine->renderFullQuad();
		//
		//gRenderEngine->activeTexture(0);

		gBuffer->resolveToScreen();

		gameGui->render();

		auto it = frameObjects.begin();
		auto eit = frameObjects.end();

		for (it; it != eit; it++) {
			(*it)->render();
		}
	} else {
		renderAsteroids();
	}
}

void Game::renderSky() {
	SkyCommand* sc = ts.allocate<SkyCommand>(1, sky, vpMat, camera.getPos(), camInput.direction(), nullptr);
	batchTmp->addCommand(sc);
	//sky->render(vpMat, camera.getPos(), camInput.direction());
}

void Game::renderScene() {

	glm::mat4 objMat;

	objMat = glm::rotate(meshRotation, glm::vec3(0, 0, 1));

	// render scene as normal
	gBufferShader->useShader();
	gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &vpMat);
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &objMat);
	gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &glm::vec4());

	glm::mat4 arr[200];
	if (mesh->hasAnimations()) {
		unsigned int numJoints = 0;
		mesh->getAnimData()->updateAnimation(0);
		void* data = mesh->getAnimData()->getCurrentAnimation(numJoints);
		memcpy(arr, data, sizeof(glm::mat4) * numJoints);
	}
	gBufferShader->bindDataArray(skinArray, UniformDataType::UNI_MATRIX4X4, arr, 200);

	mesh->bind();
	//mesh->render();

	aabbTest->bind();

	//glm::mat4 obmat2 = glm::transpose(glm::translate(engine->getAABB(0).origin));
	//obmat2 = glm::scale(obmat2, glm::vec3(2.0F));
	//gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &obmat2);
	//
	//aabbTest->render();
	//
	//obmat2 = glm::transpose(glm::translate(engine->getAABB(1).origin));
	//gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &obmat2);

	aabbTest->render();

	//gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::transpose(glm::translate(glm::mat4(), glm::vec3(0, 0, 0))));
	//mesh->bind();
	//mesh->render();

	// render mirror
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirror->modelMatrix());
	mirror->render();
	//
	// stencil test must pass before rendering
	gRenderEngine->setStencilTest(true);
	gRenderEngine->stencilFunc(FuncConstants::EQUAL, 0x01, 0xFF);
	// clear depth where mirror was written
	engine->writeDepth(1.0F, vpMat, mirror->modelMatrix());
	gRenderEngine->forceWriteDepth(true);
	mirror->render(true);
	gRenderEngine->forceWriteDepth(false);

	// reflect the sky
	sky->render(vpMat, camera.getPos(), camInput.direction(), &mirror->reflectionMatrix());

	// render reflected scene
	glm::mat4 mirrorMat = mirror->reflectionMatrix();
	glm::vec4 normal = mirror->getNormal();
	//normal.w = 20;

	gBufferShader->useShader();
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirrorMat);
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &objMat);
	gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &normal);

	mesh->bind();
	mesh->render();

	//mirrorMat = mirror->reflectionMatrix();
	normal = mirror->getNormal();
	//normal.w = -20;
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::transpose(glm::translate(glm::mat4(), glm::vec3(-20, 0, 0))));
	//gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirrorMat);
	//gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &normal);
	mesh->render();

	gRenderEngine->setStencilTest(false);

}

void Game::renderShadowMap() {

	gRenderEngine->updateViewPort(1024 * 4, 1024 * 4);
	shadowMap->bind();
	shadowMap->clear();

	glm::vec3 lightDir = glm::vec3(1, 0, 0);// sunMoonDir;

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -50, 10);
	glm::mat4 depthViewMatrix = glm::lookAt(-lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	shadowShader->useShader();
	shadowShader->bindData(shadowMVP, UniformDataType::UNI_MATRIX4X4, &depthMVP);

	gRenderEngine->enableCulling(true, false);

	mesh->bind();
	mesh->render();

	//depthModelMatrix = glm::transpose(glm::translate(glm::mat4(), glm::vec3(-20, 0, 0)));
	//depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	//
	//shadowShader->bindData(shadowMVP, UniformDataType::UNI_MATRIX4X4, &depthMVP);
	//mesh->render();

	gRenderEngine->enableCulling(false, false);
}

void Game::renderAsteroids() {

	gBuffer->bind();
	gBuffer->clear();

	gRenderEngine->setDepthTest(false);

	// ACTUAL RENDERING

	glm::vec3 white(1.0F);

	glm::mat4 mdlMat(1.0F);

	mdlMat *= glm::rotate(asteroidShip->rot, glm::vec3(0, 1, 0));

	mdlMat *= glm::transpose(glm::translate(glm::mat4(), glm::vec3(asteroidShip->xPos, 0, asteroidShip->yPos)));

	//mdlMat = glm::scale(mdlMat, glm::vec3(10.0F));

	asteroidShader->useShader();
	asteroidShader->bindData(asteroidVP, UniformDataType::UNI_MATRIX4X4, &vpMat);
	asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
	asteroidShader->bindData(asteroidColor, UniformDataType::UNI_FLOAT3, &white);

	asteroidShip->render();

	bulletMesh->bind();

	size_t nBullets = bullets.size();
	for (size_t i = 0; i < nBullets; i++) {

		mdlMat = glm::transpose(glm::translate(glm::mat4(), glm::vec3(bullets[i]->xPos, 0, bullets[i]->yPos)));
		asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
		bulletMesh->render();
	}

	size_t nAsteroids = asteroids.size();

	for (size_t i = 0; i < nAsteroids; i++) {
		mdlMat = glm::mat4();
		mdlMat *= glm::rotate(asteroids[i]->rot, glm::vec3(0, 1, 0));
		mdlMat *= glm::transpose(glm::translate(glm::mat4(), glm::vec3(asteroids[i]->xPos, 0, asteroids[i]->yPos)));
		asteroidShader->bindData(asteroidMdl, UniformDataType::UNI_MATRIX4X4, &mdlMat);
		asteroids[i]->render();
	}

	// END ACTUAL RENDERING


	int tex = 0;
	gBufferBlit->useShader();
	gBufferBlit->bindData(blitTexDiff, UniformDataType::UNI_INT, &tex);
	tex = 1;
	gBufferBlit->bindData(blitTexNorm, UniformDataType::UNI_INT, &tex);
	tex = 2;
	gBufferBlit->bindData(blitTexWPos, UniformDataType::UNI_INT, &tex);
	tex = 3;
	gBufferBlit->bindData(blitTextShadow, UniformDataType::UNI_INT, &tex);

	glm::vec3 lightDir = glm::vec3(1, 0, 0);//sunMoonDir;

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -50, 10);
	glm::mat4 depthViewMatrix = glm::lookAt(-lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	gBufferBlit->bindData(blitDepthMvp, UniformDataType::UNI_MATRIX4X4, &depthBiasMVP);

	gBufferBlit->bindData(blitEyePos, UniformDataType::UNI_FLOAT3, &camera.getPos());

	//gRenderEngine->activeTexture(0);
	//gBuffer->bindAttachment(0);
	//gRenderEngine->activeTexture(1);
	//gBuffer->bindAttachment(1);
	//gRenderEngine->activeTexture(2);
	//gBuffer->bindAttachment(2);
	//gRenderEngine->activeTexture(3);
	//shadowMap->bindAttachment(1);
	//
	//gRenderEngine->bindDefaultFrameBuffer();
	//engine->renderFullQuad();

	gBuffer->resolveToScreen();

	gRenderEngine->activeTexture(0);

	//gBuffer->resolveToScreen();

	gameGui->render();

}

/*

Private Functions

*/

void Game::updateFps(float dt) {
	dtOneSec += dt;
	fpsCounter++;

	if (dtOneSec > 1.0F) {
		fps = fpsCounter;
		fpsCounter = 0;
		dtOneSec -= 1.0F;
	}
}

void Game::updateMenu(float dt) {

	if (menu) {
		menu->update(dt);

		int pressedIndex = menu->buttonPressed();

		if (pressedIndex == 0) {
			engine->lockCursor(true);
			currentState = GameState::PLAY;
		} else if (pressedIndex == 1) {
			currentState = GameState::EDIT;
		} else if (pressedIndex == 2) {
			engine->close();
		} else if (pressedIndex == 3) {
			currentState = GameState::PLAY_ASTEROIDS;

			camInput.setCam(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0));

			int w = 0;
			int h = 0;
			Input::Input::GetInput()->getWindowSize(w, h);

			*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::ortho(float(-w), float(w), float(-h), float(h));

			//			*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::ortho(-100.0F, 100.0F, -100.0F, 100.0F);

			gRenderEngine->setClearColor(0.0F, 0.0F, 0.0F, 1.0F);

			for (size_t i = 0; i < 3; i++) {
				Asteroid* a = new Asteroid(ASTEROID_SIZE::LARGE);
				asteroids.push_back(a);
			}
		}

		frameObjects.push_back(menu->getRenderable());
	}

}

const float MAX_ROT_SPEED = toRADIAN * 36.0F;

void Game::updatePlay(float dt) {

	camInput.update(dt);

	player->update(dt);

	Input::Input* in = Input::Input::GetInput();

	bool hadInput = false;

	if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_LEFT_ARROW])) {
		rotSpeed += 2.0F * dt;
		hadInput = true;
	}

	if (in->isKeyBindPressed(Input::KeyBindings[Input::KEYBIND_RIGHT_ARROW])) {
		rotSpeed -= 2.0F * dt;
		hadInput = true;
	}

	if (!hadInput) {

		if (abs(rotSpeed) < 0.01F) {
			rotSpeed = 0.0F;
		} else {
			if (rotSpeed > 0.0) {
				rotSpeed -= dt;
			} else {
				rotSpeed += dt;
			}
		}
	} else {
		rotTime += dt;
	}

	rotSpeed = min(max(rotSpeed, -MAX_ROT_SPEED), MAX_ROT_SPEED);

	meshRotation += rotSpeed * dt;

}

void Game::updateEdit(float dt) {

	const bool freeCam = true;

	(*editor)->update(dt);

	if ((*editor)->mouseInGui() == false) {
		camInput.update(dt, freeCam);
	}

	Renderable** rends = (*editor)->renderObjects(1);

	frameObjects.push_back(rends[0]);
}

void Game::updateAsteroids(float dt) {
	//camInput.update(dt);

	asteroidShip->update(dt);

	size_t nBullets = bullets.size();

	for (size_t i = 0; i < nBullets; i++) {
		bullets[i]->update(dt);
	}

	auto it = bullets.begin();
	auto eit = bullets.end();

	while (it != eit) {
		Bullet* b = *it;

		if (b->dead()) {
			delete b;
			it = bullets.erase(it);
			eit = bullets.end();
		} else {
			it++;
		}
	}

	size_t nAsteroids = asteroids.size();

	for (size_t i = 0; i < nAsteroids; i++) {
		asteroids[i]->update(dt);
	}

	nBullets = bullets.size();

	for (size_t i = 0; i < nAsteroids; i++) {

		for (size_t j = 0; j < nBullets; j++) {

			if (asteroids[i]->collide(bullets[j]->xPos, bullets[j]->yPos)) {

				bullets[j]->ttl = -1.0F;

				printf("Collision\n");

			}
		}
	}

	auto ait = asteroids.begin();
	auto aeit = asteroids.end();

	while (ait != aeit) {
		Asteroid* a = *ait;

		if (a->isDead()) {
			ait = asteroids.erase(ait);

			if (a->mSize == ASTEROID_SIZE::LARGE) {
				for (size_t i = 0; i < 3; i++) {
					Asteroid* ad = new Asteroid(ASTEROID_SIZE::MEDIUM, a->xPos, a->yPos);
					asteroids.push_back(ad);
				}
			}

			if (a->mSize == ASTEROID_SIZE::MEDIUM) {
				for (size_t i = 0; i < 3; i++) {
					Asteroid* ad = new Asteroid(ASTEROID_SIZE::SMALL, a->xPos, a->yPos);
					asteroids.push_back(ad);
				}
			}

			delete a;
			ait = asteroids.begin();
			aeit = asteroids.end();
		} else {
			ait++;
		}
	}

	float sx = 0, sy = 0;

	if (asteroidShip->getShootPoint(sx, sy)) {

		Bullet* b = new Bullet(sx, sy, asteroidShip->rot);

		bullets.push_back(b);

	}

}


void Game::updatePaused(float dt) {

}