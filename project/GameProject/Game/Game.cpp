
/// Internal Includess
#include "Game.hpp"
#include "../Engine/Graphics/Graphics.hpp"

#include <glm/gtx/transform.hpp>

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

	*(glm::mat4*)(camera.getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0F), 1280.0F, 720.0F, 0.01F, 100.0F);

	mesh = new Engine::Graphics::Mesh::StaticMesh();
	mesh->loadMesh("Data/Meshes/Test_exteriorScene_vColor.mesh");
	gameGui = new Engine::Graphics::CGui();
	gameGui->setVisible(true);

	panelTexture = new Engine::Graphics::Texture::Texture2D();
	panelTexture->singleColor(0.5F, 0.5F, 0.5F, 0.5F);

	metrixPanel = new Engine::Graphics::Gui::Panel();
	metrixPanel->setPosition(10, 10);
	metrixPanel->setSize(300, 50);
	metrixPanel->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_RIGHT);
	metrixPanel->setVisible(true);
	metrixPanel->setTexture(panelTexture);

	infoLabel = new Engine::Graphics::Gui::Label();
	infoLabel->setSize(300, 50);
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
	fbci.mutlisample = 0;
	fbci.nrColorBuffers = 3;
	fbci.useDepth = true;
	fbci.useMultisample = false;
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

	if (gameGui) {
		delete gameGui;
		gameGui = nullptr;
	}
	delete metrixPanel;
	delete infoLabel;
	delete panelTexture;
	
}

void Game::update(float dt) {
	engine->update(dt);
	gameGui->update(dt);
	camInput.update(dt);

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

	//a += 0.3F * dt;

	std::string str = "FPS: " + std::to_string(fps) + "\n";
	str += "Dt: " + std::to_string(dt) + "\n";

	glm::vec3 pos = camera.getPos();
	str += "Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "\n";

	glm::vec3 dir = camInput.direction();
	str += "Dir: (" + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z) + "\n";

	infoLabel->setText(str.c_str());
}

void Game::render() {

	gBuffer->bind();
	gBuffer->clear();

	gBufferShader->useShader();
	gBufferShader->bindData(vpLocationGBuff, UniformDataType::UNI_MATRIX4X4, &vpMat);
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gBufferShader->bindData(selectedLocGBuff, UniformDataType::UNI_FLOAT3, &glm::vec3(0.4F));
	
	// render mirror
	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirror->modelMatrix());
	mirror->render();

	gRenderEngine->setStencilTest(false);
	gRenderEngine->stencilFunc(0x0205, 0x01, 0xFF);

	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());

	mesh->bind();
	mesh->render();

	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::transpose(glm::translate(glm::mat4(), glm::vec3(-20, 0, 0))));
	gRenderEngine->setStencilTest(true);

	mesh->bind();
	mesh->render();

	glm::mat4 mirrorMat = mirror->reflectionMatrix();

	gRenderEngine->setStencilTest(true);
	gRenderEngine->stencilFunc(0x0202, 0x01, 0xFF);

	gBufferShader->bindData(matLocationGBuff, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	gBufferShader->bindData(refMatLocationGBuff, UniformDataType::UNI_MATRIX4X4, &mirrorMat);
	mesh->bind();
	mesh->render();

	gRenderEngine->setStencilTest(false);

	int index = 0;

	gBuffer->resolveToScreen(index);
	gBuffer->resolveAllToScreen();

	gameGui->render();

}