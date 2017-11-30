
/// Internal Includess
#include "Game.hpp"
#include "../Engine/Graphics/Graphics.hpp"

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
// @temporary end

Game::Game(CEngine* _engine)
	: paused(false)
	, gameGui(nullptr)
	, engine(_engine)
	, mesh(nullptr)
	, dtOneSec(0.0F)
	, fps(0)
	, fpsCounter(0) {

	camInput.init((glm::mat4*)camera.getViewMatrix());
	camInput.setCam(glm::vec3(5, 1, 0), glm::vec3(-1, 0, 0));

	*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0F), 1280.0F, 720.0F, 0.1F, 100.0F);

	mesh = new Engine::Graphics::Mesh::StaticMesh();
	mesh->loadMesh("Data/Meshes/Test_exteriorScene_vColor.mesh");
	gameGui = new Engine::Graphics::CGui();
	gameGui->setVisible(true);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	metrixPanel = new Engine::Graphics::Gui::Panel();
	metrixPanel->setPosition(-10, 10);
	metrixPanel->setSize(300, 100);
	metrixPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	metrixPanel->setVisible(true);
	metrixPanel->setTexture(panelTexture);

	infoLabel = new Engine::Graphics::Gui::Label();
	infoLabel->setSize(300, 100);
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

	FrameBufferCreateInfo fbci{};

	fbci.height = 720;
	fbci.width = 1280;
	fbci.mutlisample = 16;
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

	camPath.init(&camInput);
	camPath.followPaths(true);

	skyDome = new SkyDome();

	skyDomeShader = gRenderEngine->createShaderObject();
	skyDomeShader->init();

	vs = readShader("data/shaders/skydome.vs.glsl");
	fs = readShader("data/shaders/skydome.fs.glsl");

	skyDomeShader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	skyDomeShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
	skyDomeShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!skyDomeShader->buildShader()) {
		assert(0 && "shader failed to build");
	}

	skydomeVpLocation = skyDomeShader->getShaderUniform("viewProjMatrix");
	skydomeMatLocation = skyDomeShader->getShaderUniform("worldMat");
	skydomeTimeLoc = skyDomeShader->getShaderUniform("time");
	skydomeCamPos = skyDomeShader->getShaderUniform("cameraPos");
	skydomeEyeDir = skyDomeShader->getShaderUniform("eyeDir");
	skydomeSunMoon = skyDomeShader->getShaderUniform("sunMoonDir");

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

}

Game::~Game() {

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	delete mirror;

	shader->release();
	gBuffer->release();
	gBufferShader->release();
	skyDomeShader->release();
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
	delete skyDome;
}

void Game::update(float dt) {
	engine->update(dt);
	gameGui->update(dt);
	camInput.update(dt);
	camPath.update(dt);


	if (GetAsyncKeyState(VK_F8)) {

		std::string vs = readShader("data/shaders/skydome.vs.glsl");
		std::string fs = readShader("data/shaders/skydome.fs.glsl");

		skyDomeShader->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
		skyDomeShader->setShaderCode(ShaderStages::GEOMETRY_STAGE, nullptr);
		skyDomeShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

		skyDomeShader->buildShader();

		skydomeVpLocation = skyDomeShader->getShaderUniform("viewProjMatrix");
		skydomeMatLocation = skyDomeShader->getShaderUniform("worldMat");
		skydomeTimeLoc = skyDomeShader->getShaderUniform("time");
		skydomeCamPos = skyDomeShader->getShaderUniform("cameraPos");
		skydomeEyeDir = skyDomeShader->getShaderUniform("eyeDir");
		skydomeSunMoon = skyDomeShader->getShaderUniform("sunMoonDir");
		Sleep(10);
	}

	if (Input::Input::GetInput()->sizeChange) {
		int w = 0;
		int h = 0;
		Input::Input::GetInput()->getWindowSize(w, h);
		gBuffer->resize(w, h);
		gBuffer->setWindowSize(w, h);
	}

	vpMat = camera.viewProjection();

	dtOneSec += dt;
	fpsCounter++;

	float x = r * glm::sin(a);
	float z = r * glm::cos(a);

	glm::vec3 posx = glm::vec3(x, 2, z);

	mirror->setMirrorPosition(posx);
	mirror->setMirrorNormal(-glm::normalize(posx - glm::vec3(0, 2, 0)));
	mirror->setSize(glm::vec2(10, 5));

	if (dtOneSec > 1.0F) {
		fps = fpsCounter;
		fpsCounter = 0;
		dtOneSec -= 1.0F;

	}

	skyTime += dt * 0.01F;
	if (skyTime > 1.0F) {
		skyTime -= 1.0F;
	}
	//skyTime = 0.5F;
	//a += 0.3F * dt;


	const float r = 100.0F;
	const float PI = glm::pi<float>();

	float s = (glm::abs(glm::sin(PI * skyTime * 2.0F + (PI * 0.5F))) * 1.6F) - 0.6F;
	float c = glm::cos(PI * skyTime * 2.0F + (PI * 0.5F));

	if (skyTime >= 0.25F && skyTime <= 0.75F) {
		c = -c;
	}
	if (s > 0.0F) {
		s *= 0.2F;
	}

	sunMoonDir = glm::vec3(-r * 0.4F, r * s, r * c);

	sunMoonDir = normalize(glm::vec3(0) - sunMoonDir);


	std::string str = "FPS: " + std::to_string(fps) + "\n";
	str += "Dt: " + std::to_string(dt) + "\n";

	glm::vec3 pos = camera.getPos();
	str += "Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "\n";

	glm::vec3 dir = camInput.direction();
	str += "Dir: (" + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z) + "\n";

	str += "SkyTime: " + std::to_string(skyTime) + "\n";

	//const float r = 100.0F;
	//glm::vec3 sunMoonDir(-r * 0.47F, r * sin(glm::pi<float>() * skyTime), r * sin(glm::pi<float>() * skyTime));
	//
	//sunMoonDir = normalize(glm::vec3(0) - sunMoonDir);
	//
	//str += "Sun: (" + std::to_string(sunMoonDir.x) + ", " + std::to_string(sunMoonDir.y) + ", " + std::to_string(sunMoonDir.z) + "\n";

	infoLabel->setText(str.c_str());
}

void Game::render() {

	renderShadowMap();
	int w = 0;
	int h = 0;
	Input::Input::GetInput()->getWindowSize(w, h);
	gRenderEngine->updateViewPort(w, h);

	gBuffer->bind();
	gBuffer->clear();

	renderSky();

	renderScene();

	int index = 0;

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

	glm::vec3 lightDir = sunMoonDir;

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

	gRenderEngine->activeTexture(0);
	gBuffer->bindAttachment(0);
	gRenderEngine->activeTexture(1);
	gBuffer->bindAttachment(1);
	gRenderEngine->activeTexture(2);
	gBuffer->bindAttachment(2);
	gRenderEngine->activeTexture(3);
	shadowMap->bindAttachment(1);

	gRenderEngine->bindDefaultFrameBuffer();
	engine->renderFullQuad();

	gRenderEngine->activeTexture(0);

	gameGui->render();

}

void Game::renderSky() {
	skyDomeShader->useShader();
	skyDomeShader->bindData(skydomeVpLocation, UniformDataType::UNI_MATRIX4X4, &vpMat);
	skyDomeShader->bindData(skydomeMatLocation, UniformDataType::UNI_MATRIX4X4, &glm::transpose(glm::translate(glm::mat4(), camera.getPos() - glm::vec3(0, 1, 0))));
	skyDomeShader->bindData(skydomeTimeLoc, UniformDataType::UNI_FLOAT, &skyTime);
	skyDomeShader->bindData(skydomeCamPos, UniformDataType::UNI_FLOAT3, &camera.getPos());
	skyDomeShader->bindData(skydomeEyeDir, UniformDataType::UNI_FLOAT3, &camInput.direction());

	skyDomeShader->bindData(skydomeSunMoon, UniformDataType::UNI_FLOAT3, &sunMoonDir);

	gRenderEngine->depthMask(false);

	skyDome->render();

	gRenderEngine->depthMask(true);
}

void Game::renderScene() {

	// render scene as normal
	gBufferShader->useShader();
	gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &vpMat);
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gBufferShader->bindData(clipPlane, UniformDataType::UNI_FLOAT4, &glm::vec4());

	mesh->bind();
	mesh->render();

	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::transpose(glm::translate(glm::mat4(), glm::vec3(-20, 0, 0))));
	mesh->bind();
	mesh->render();

	// render mirror
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirror->modelMatrix());
	mirror->render();

	// stencil test must pass before rendering
	gRenderEngine->setStencilTest(true);
	gRenderEngine->stencilFunc(FuncConstants::EQUAL, 0x01, 0xFF);
	// clear depth where mirror was written
	engine->writeDepth(1.0F, vpMat, mirror->modelMatrix());
	gRenderEngine->forceWriteDepth(true);
	mirror->render(true);
	gRenderEngine->forceWriteDepth(false);

	// reflect the sky
	skyDomeShader->useShader();
	skyDomeShader->bindData(skydomeVpLocation, UniformDataType::UNI_MATRIX4X4, &vpMat);

	glm::mat4 mat = glm::transpose(glm::translate(glm::mat4(), camera.getPos() - glm::vec3(0, 1, 0)));

	skyDomeShader->bindData(skydomeMatLocation, UniformDataType::UNI_MATRIX4X4, &mat);
	skyDomeShader->bindData(skydomeTimeLoc, UniformDataType::UNI_FLOAT, &skyTime);

	glm::vec4 pos = glm::vec4(camera.getPos(), 1.0F);// *mirror->reflectionMatrix();

	glm::vec3 dir = glm::reflect(camInput.direction(), glm::vec3(mirror->getNormal()));

	skyDomeShader->bindData(skydomeCamPos, UniformDataType::UNI_FLOAT3, &pos);
	skyDomeShader->bindData(skydomeEyeDir, UniformDataType::UNI_FLOAT3, &mirror->getNormal());

	skyDomeShader->bindData(skydomeSunMoon, UniformDataType::UNI_FLOAT3, &sunMoonDir);

	gRenderEngine->depthMask(false);

	skyDome->render();

	gRenderEngine->depthMask(true);


	// render reflected scene
	glm::mat4 mirrorMat = mirror->reflectionMatrix();
	glm::vec4 normal = mirror->getNormal();
	//normal.w = 20;

	gBufferShader->useShader();
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirrorMat);
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
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

	gRenderEngine->updateViewPort(1024 *4, 1024 *4);
	shadowMap->bind();
	shadowMap->clear();

	glm::vec3 lightDir = sunMoonDir;

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
