#include "Game.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

#include "../Core/CoreGlobals.hpp"
#include "../Core/Input/Input.hpp"
#include "../Core/AssetManager.hpp"

#include <AssetLib/AssetLib.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
//@Temporary

bool sphereInRay(glm::vec3 const & rayStart, glm::vec3 const & rayDir, float const radius, glm::mat4 const &mat) {

	glm::vec3 spherePos = glm::vec3(mat[0][3], mat[1][3], mat[2][3]);

	glm::vec3 m = rayStart - spherePos;
	float b = glm::dot(m, rayDir);
	float c = glm::dot(m, m) - (radius * radius);

	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float discr = (b*b) - c;
	if (discr < 0.0f) {
		return false;
	}

	return true;
}

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


GameObject* createGo(AssetManager* assetManager, std::string meshName, float x, float y, float z) {

	GameObject* go = new GameObject();
	uint32_t instanceID = assetManager->loadMesh((char*)meshName.c_str());
	RenderComponent* rc = new RenderComponent();
	rc->init();
	rc->setInstanceId(instanceID);
	go->addComponent(rc);
	
	TransformComponent* tc = new TransformComponent();
	tc->init();
	tc->setPosition(glm::vec3(x, y, z));
	go->addComponent(tc);
	
	return go;
}

//@EndTemporary

Game::Game() : running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu) {}

Game::~Game() {
	
	std::vector<GameObject*>::const_iterator it = gameObjects.begin();
	std::vector<GameObject*>::const_iterator eit = gameObjects.end();

	for (it; it != eit; it++) {
		if (*it) {
			delete *it;
		}
	}

	shdr->release();
	assetManager->freeResources();
	delete assetManager;
	delete cam;
	delete core;
}

void Game::init() {
	core = new Core();
	core->init();

	cam = new Camera();

	assetManager = new AssetManager();
	assetManager->init();
	
	camInput.init((glm::mat4*)cam->getViewMatrix());
	*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);
	*(glm::mat4*)(cam->getOrthoMatrix()) = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);

	gstate = GameState::eGameStage_MainMenu;

	gstate = GameState::eGameState_PlayMode;

	gameObjects.push_back(createGo(assetManager, "data/meshes/test.mesh", 0, 0, 0));

	core->startEditor();

	std::string vs = readShader("data/shaders/default.vs.glsl");
	std::string gs = readShader("data/shaders/default.gs.glsl");
	std::string fs = readShader("data/shaders/default.fs.glsl");

	shdr = core->getRenderEngine()->createShaderObject();
	shdr->init();

	shdr->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	shdr->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	shdr->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!shdr->buildShader()) {
		printf("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	vpLocation = shdr->getShaderUniform("viewProjMatrix");
	matLocation = shdr->getShaderUniform("worldMat");
	selectedLoc = shdr->getShaderUniform("selectedColor");

}

bool Game::isRunning() const {
	return running;
}

void Game::updateAndRender(float dt) {

	core->update(dt);
	tickFPS(dt);
	running = core->isRunning();

	Input* in = Input::getInput();
	if (in->sizeChange) {
		int w = 0, h = 0;
		in->getWindowSize(w, h);
		if (w != 0 && h != 0) {
			*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(w), float(h), 0.0001f, 100.0f);
		}
	}
	
	switch (gstate) {
		case GameState::eGameStage_MainMenu:
			updateMenu(dt);
			break;
		case GameState::eGameState_EditorMode:
			updateEditor(dt);
			break;
		case GameState::eGameState_PlayMode:
			updateGame(dt);
			break;
		case GameState::eGameState_loading:
			updateLoading(dt);
			break;
		case GameState::eGameState_Undefined:
		default:
			break;
	}

	//core->syncThreads();

	render();
}

void Game::render() {
	glm::mat4 vp = *(glm::mat4*)cam->getPerspectiveMatrix() * *(glm::mat4*)cam->getViewMatrix();

	core->render(vp);

	std::vector<GameObject*>::const_iterator it = gameObjects.begin();
	std::vector<GameObject*>::const_iterator eit = gameObjects.end();

	shdr->useShader();
	shdr->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, &vp);
	shdr->bindData(selectedLoc, UniformDataType::UNI_FLOAT3, &glm::vec3(1.0f));

	for (it; it != eit; it++) {
		if (*it) {
			RenderComponent* rc = (RenderComponent*)(*it)->getComponent<RenderComponent>();
			TransformComponent* tc = (TransformComponent*)(*it)->getComponent<TransformComponent>();

			if (rc != nullptr) {
				IMesh* mesh = assetManager->getMeshFromInstanceId(rc->getInstanceId());
				if (mesh != nullptr) {

					glm::mat4 mdl = tc->getModelMatrix();
					shdr->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &mdl);
					mesh->bind();
					mesh->render();
				}
			}
		}
	}

	core->swap();
}

void Game::updateMenu(float dt) {}

void Game::updateEditor(float dt) {

}

void Game::updateGame(float dt) {
	camInput.update(dt);
}

void Game::updateLoading(float dt) {

}

void Game::tickFPS(float dt) {
	fps++;
	timepass += dt;

	if (timepass > 1.0f) {
		ffps = fps;
		fps = 0;
		timepass -= 1.0f;
	}
}