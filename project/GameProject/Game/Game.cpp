
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

}

Game::~Game() {

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	shader->release();

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

	if (dtOneSec > 1.0F) {
		fps = fpsCounter;
		fpsCounter = 0;
		dtOneSec -= 1.0F;
	}

	std::string str = "FPS: " + std::to_string(fps) + "\n";
	str += "Dt: " + std::to_string(dt) + "\n";

	glm::vec3 pos = camera.getPos();
	str += "Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "\n";

	glm::vec3 dir = camInput.direction();
	str += "Dir: (" + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z) + "\n";

	infoLabel->setText(str.c_str());
}

void Game::render() {

	shader->useShader();
	shader->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, &vpMat);
	shader->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	shader->bindData(selectedLoc, UniformDataType::UNI_FLOAT3, &glm::vec3(0.4F));

	mesh->bind();
	mesh->render();

	gameGui->render();

}