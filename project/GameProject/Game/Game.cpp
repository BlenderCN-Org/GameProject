
/// Internal Includess
#include "Game.hpp"
#include "../Engine/Core/System.hpp"
#include "../Engine/Graphics/Graphics.hpp"

/// External Includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

/// Std Includes
#include <fstream>
#include <string>

// @temporary

TemporaryStorage ts(16 * KB);

// @temporary end

Game::Game(CEngine* _engine)
	: gameGui(nullptr)
	, engine(_engine)
	, mesh(nullptr)
	, batchTmp(nullptr) {

	currentState = GameState::MAIN_MENU;
	lastState = GameState::MAIN_MENU;

	camInput.init((glm::mat4*)camera.getViewMatrix());
	camInput.setCam(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1));

	//player = new Player();
	//player->setCamera(&camInput);

	*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0F), 1280.0F, 720.0F, 0.1F, 100.0F);

	mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/Test_exteriorScene_vColor1.mesh");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/V2/Creature.mesh");
	//aabbTest = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/Test_exteriorScene_vColor1.mesh");
	aabbTest = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/V2/HalfCube.mesh");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/newFmat.mesh");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/untitled.dae");
	//mesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadDea("Data/Meshes/untitled.dae");

	//mesh = new Engine::Graphics::Mesh::CMesh();
	//mesh->loadMesh("Data/Meshes/Test_exteriorScene_vColor.mesh");
	//mesh->loadMesh("Data/Meshes/newFmat.skel");

	viewGizmo = new ViewGizmo(engine);

	engine->setCursor("Data/Textures/Gui/Cursor2.png");

	gameGui = new Engine::Graphics::CGui();
	gameGui->setVisible(true);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);


	metrixPanel = new Engine::Graphics::Gui::Panel();
	metrixPanel->setPosition(-10, 10);
	metrixPanel->setSize(400, 200);
	metrixPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	metrixPanel->setVisible(true);
	metrixPanel->setTexture(panelTexture);

	infoLabel = new Engine::Graphics::Gui::Label();
	infoLabel->setSize(400, 200);
	infoLabel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	infoLabel->setVisible(true);
	infoLabel->setText("test");

	gameGui->addGuiItem(metrixPanel);
	metrixPanel->addGuiItem(infoLabel);

	shader = gRenderEngine->createShaderObject();
	shader->init();

	std::string vs = Engine::System::readShader("data/shaders/default.vs.glsl");
	std::string gs = Engine::System::readShader("data/shaders/default.gs.glsl");
	std::string fs = Engine::System::readShader("data/shaders/default.fs.glsl");

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

	vs = Engine::System::readShader("data/shaders/gbuffer.vs.glsl");
	gs = Engine::System::readShader("data/shaders/gbuffer.gs.glsl");
	fs = Engine::System::readShader("data/shaders/gbuffer.fs.glsl");

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

	//sky = new Sky();

	gBufferBlit = gRenderEngine->createShaderObject();
	gBufferBlit->init();

	vs = Engine::System::readShader("data/shaders/BlitShader.vs.glsl");
	fs = Engine::System::readShader("data/shaders/BlitShader.fs.glsl");

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

	vs = Engine::System::readShader("data/shaders/shadow.vs.glsl");
	fs = Engine::System::readShader("data/shaders/shadow.fs.glsl");

	shadowShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	shadowShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	shadowShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!shadowShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	shadowMVP = shadowShader->getShaderUniform("depthMVP");

	menu = new MainMenu();

	editor = new Editor();

	mapLoader.openMap("E:/GameProjectAssets/Data/GameMap.map");

	LoadedData data = mapLoader.loadDataEntry(1);

	map = new Map(data, mapLoader);

	free(data.data);

	meshRotation = 0.0F;

	physParticles.push_back(new PhysicsParticle(engine->getPhysEngine()));

	testShader = gRenderEngine->createShaderObject();

	vs = Engine::System::readShader("data/shaders/test/test.vs.glsl");
	fs = Engine::System::readShader("data/shaders/test/test.fs.glsl");

	testShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	testShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	testShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!testShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

}

Game::~Game() {

	delete viewGizmo;

	testShader->release();

	for (size_t i = 0; i < physParticles.size(); i++) {
		PhysicsParticle* p = physParticles[i];
		delete p;
	}

	physParticles.empty();

	delete map;

	delete editor;

	delete menu;

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
	//delete sky;

	//delete player;
}

void Game::update(float dt, uint64_t clocks) {

	for (size_t i = 0; i < physParticles.size(); i++) {
		PhysicsParticle* p = physParticles[i];
		if (p->sholdRemove()) {
			auto it = std::find(physParticles.begin(), physParticles.end(), physParticles[i]);
			physParticles.erase(it);
			delete p;
		}
	}

	for (size_t i = 0; i < 0; i++) {
		physParticles.push_back(new PhysicsParticle(engine->getPhysEngine()));
	}

	size_t tUsed = ts.getUsed();

	ts.clear();

	batchTmp = ts.allocate<RenderBatch>(1, 1000, &ts);

	frameObjects.clear();

	engine->update(dt);
	fpsCounter.update(dt);

	gameGui->update(dt);

	if (Engine::Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Engine::Input::Input::GetInput()->getWindowSize(w, h);
		gBuffer->resize(w, h);
		gBuffer->setWindowSize(w, h);
	}

	//ts.

	bool clear = true;

	ActiveFrameBufferCommand* fbCmd = ts.allocate<ActiveFrameBufferCommand>(1, gBuffer, clear);
	batchTmp->addCommand(fbCmd);

	batchTmp->setCameraSettings({ vpMat, camera.getPos(), camInput.direction() });

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

	//sky->update(dt);

	float x = r * glm::sin(a);
	float z = r * glm::cos(a);

	glm::vec3 posx = glm::vec3(x, 2, z);

	mirror->setMirrorPosition(posx);
	mirror->setMirrorNormal(-glm::normalize(posx - glm::vec3(0, 2, 0)));
	mirror->setSize(glm::vec2(10, 5));


	std::string str = "FPS: " + std::to_string(fpsCounter.getFPS()) + "\n";
	str += "Dt: " + std::to_string(dt) + "\n";

	glm::vec3 pos = camera.getPos();
	str += "Pos: ( X:" + std::to_string(pos.x) + ", Y:" + std::to_string(pos.y) + ", Z:" + std::to_string(pos.z) + ")\n";

	glm::vec3 dir = camInput.direction();
	str += "Dir: ( X:" + std::to_string(dir.x) + ", Y:" + std::to_string(dir.y) + ", Z:" + std::to_string(dir.z) + ")\n";

	str += "TempUsed: " + std::to_string(tUsed) + " Bytes\n";

	str += "Clocks: " + std::to_string(clocks) + "\n";

	str += "RotationSpeed: " + std::to_string(rotSpeed) + "\n";

	str += "Rotation: " + std::to_string(meshRotation * toDEGREE) + " (" + std::to_string((meshRotation * toDEGREE) / 360.0F) + ")\n";
	str += "RotTime: " + std::to_string(rotTime) + "\n";

	Engine::Input::InputEvent ie = Engine::Input::Input::GetInput()->lastPressed;

	str += "Last Key: " + std::string(ie.mouse ? "mouse " : "key ") + std::to_string(ie.code) + "\n";

	str += "Particles: " + std::to_string(physParticles.size()) + "\n";

	str += engine->getThreadManager()->getThreadInfo(0) + " / " + engine->getThreadManager()->getThreadInfo(1) + " / ";
	str += engine->getThreadManager()->getThreadInfo(2) + "\n" + engine->getThreadManager()->getThreadInfo(3) + " / ";
	str += engine->getThreadManager()->getThreadInfo(4) + " / " + engine->getThreadManager()->getThreadInfo(5) + "\n";
	str += engine->getThreadManager()->getThreadInfo(6) + " / " + engine->getThreadManager()->getThreadInfo(7) + "\n";

	//str += "SkyTime: " + std::to_string(skyTime) + "\n";

	infoLabel->setText(str.c_str());
}

void Game::render() {

	int w = 0;
	int h = 0;
	Engine::Input::Input::GetInput()->getWindowSize(w, h);
	gRenderEngine->updateViewPort(w, h);

	/*if (currentState != GameState::PLAY_ASTEROIDS) {

		//renderShadowMap();

		//gBuffer->bind();
		//gBuffer->clear();

		renderSky();

		batchTmp->executeBatch();

		//renderSky();

		gBufferShader->useShader();
		gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &vpMat);
		gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
		gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &glm::vec4());

		aabbTest->bind();

		uint32_t numRigidObjects = engine->getPhysEngine()->getNumRigidBodies();

		const RigidBody* const* rbs = engine->getPhysEngine()->getRigidObjects();

		for (uint32_t i = 0; i < numRigidObjects; i++) {

			glm::vec3 pos = rbs[i]->position;

			glm::mat4 obmat2;
			//glm::mat4 obmat2 = glm::transpose(glm::translate(pos));
			//obmat2 = glm::scale(obmat2, glm::vec3(0.1F));

			obmat2[0][3] = pos.x;
			obmat2[1][3] = pos.y;
			obmat2[2][3] = pos.z;

			obmat2[0][0] = 0.1F;
			obmat2[1][1] = 0.1F;
			obmat2[2][2] = 0.1F;

			gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &obmat2);
			aabbTest->render();
		}

		renderScene();
		*/
	if (currentState != GameState::PLAY_ASTEROIDS) {
		gBuffer->bind();
		gBuffer->clear();

		map->updateRenderBatch(*batchTmp);

		batchTmp->executeBatch();

		map->render();

		if (currentState == GameState::EDIT) {
			
			viewGizmo->gizmoMesh->bind();

			glm::mat4 viewRot = *(glm::mat4*)camera.getViewMatrix();

			viewRot = glm::inverse(viewRot);
			viewRot[3][0] = 0.0F;
			viewRot[3][1] = 0.0F;
			viewRot[3][2] = 0.0F;

			glm::mat4 objMat = glm::mat4();

			glm::vec3 camPos = camera.getPos();
			glm::vec3 dir = camInput.direction() ;

			viewRot = glm::inverse(viewRot);

			objMat[3][0] = camPos.x + (dir.x * 5);
			objMat[3][1] = camPos.y + (dir.y * 5);
			objMat[3][2] = camPos.z + (dir.z * 5);


			objMat = glm::scale(objMat, glm::vec3(0.3F));

			objMat = glm::transpose(objMat);

			glm::mat4 moveMat;
			moveMat[3][0] = -0.9F;
			moveMat[3][1] = -0.9F;


			glm::mat4 finalTranspose = moveMat * vpMat;


			gBufferShader->useShader();
			gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &finalTranspose);
			gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
			gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &glm::vec4());
			gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &objMat);

			viewGizmo->gizmoMesh->render();

			gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &vpMat);
			gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

			viewGizmo->gizmoMesh->render();

		}

	} else {
		renderAsteroids();
	}

	present();

}

/*

Private Functions

*/

void Game::renderSky() {
	//SkyCommand* sc = ts.allocate<SkyCommand>(1, sky, vpMat, camera.getPos(), camInput.direction(), nullptr);
	//batchTmp->addCommand(sc);
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
	//sky->render(vpMat, camera.getPos(), camInput.direction(), &mirror->reflectionMatrix());

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

	map->render();

}

void Game::present() {

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


	auto it = frameObjects.begin();
	auto eit = frameObjects.end();

	for (it; it != eit; it++) {
		(*it)->render();
	}

	gameGui->render();

}


void Game::updateMenu(float dt) {

	if (menu) {
		menu->update(dt);

		int pressedIndex = menu->buttonPressed();

		if (pressedIndex == 0) {
			engine->lockCursor(true);
			currentState = GameState::PLAY;

			//map->getPlayer()->setCamera(&camInput);

		} else if (pressedIndex == 1) {
			currentState = GameState::EDIT;
		} else if (pressedIndex == 2) {
			engine->close();
		} else if (pressedIndex == 3) {
			currentState = GameState::PLAY_ASTEROIDS;

			if (map) {
				delete map;
			}

			map = new AsteroidsMap(camInput, camera);

		}

		frameObjects.push_back(menu->getRenderable());
	}

	map->update(dt);

}

const float MAX_ROT_SPEED = toRADIAN * 36.0F;

void Game::updatePlay(float dt) {

	camInput.update(dt);

	//player->update(dt);

	map->update(dt);
	IPlayer* p = map->getPlayer();

	if (p) {
		p->update(dt);
	}

	Engine::Input::Input* in = Engine::Input::Input::GetInput();

	bool hadInput = false;

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_LEFT_ARROW])) {
		rotSpeed += 2.0F * dt;
		hadInput = true;
	}

	if (in->isKeyBindPressed(Engine::Input::KeyBindings[Engine::Input::KEYBIND_RIGHT_ARROW])) {
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

	editor->update(dt);

	if (editor->mouseInGui() == false) {
		camInput.update(dt, freeCam);
	}

	Renderable** rends = editor->renderObjects(1);

	frameObjects.push_back(rends[0]);
}

void Game::updateAsteroids(float dt) {
	map->update(dt);
}


void Game::updatePaused(float dt) {

}