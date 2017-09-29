#include "Game.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene.hpp"
#include "EditorAccess.hpp"

#include "../Core/CoreGlobals.hpp"
#include "../Core/Input/Input.hpp"
#include "../Core/System/Console.hpp"
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

//@EndTemporary

Game::Game() : running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu) {}

Game::~Game() {
	//shdr->release();
	delete guiPanel;
	gBuffer->release();
	gui->freeResources();

	currentScene->release();
	delete currentScene;

	delete gui;
	delete cam;
	delete core;
}

void Game::init() {
	core = new Core();
	if (!core->init()) {
		assert(0 && "Failed to initialize!");
	}

	cam = new Camera();

	gui = new Gui();
	gui->init();

	editAccess.setGameInstance(this);
	currentScene = nullptr;
	currentScene = new Scene();
	currentScene->init(gAssetManager->getStartupSceneRef());

	camInput.init((glm::mat4*)cam->getViewMatrix());
	//camInput.setCamSpeed(2.0F);
	*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);
	*(glm::mat4*)(cam->getOrthoMatrix()) = glm::ortho(0.0F, 1280.0F, 720.0F, 0.0F);

	gstate = GameState::eGameState_PlayMode;

	gBuffer = gRenderEngine->createFrameBuffer();
	FrameBufferCreateInfo fbci;
	fbci.height = 720;
	fbci.width = 1280;
	fbci.useDepth = true;
	fbci.useStencil = false;
	fbci.nrColorBuffers = 2;
	fbci.useMultisample = false;

	if (!gBuffer->init(&fbci)) {
		gConsole->print("Failed to create gBuffer");
	}

	guiPanel = new GuiPanel();
	guiPanel->setPosition(0, 0);
	guiPanel->setSize(100, 100);
	guiPanel->setAnchorPoint(GuiAnchor::TOP);

	//gameObjects.push_back(createGo2(gAssetManager, "data/meshes/test.mesh", 0, 0, 0));

	//std::string vs = readShader("data/shaders/default.vs.glsl");
	//std::string gs = readShader("data/shaders/default.gs.glsl");
	//std::string fs = readShader("data/shaders/default.fs.glsl");

	/*shdr = core->getRenderEngine()->createShaderObject();
	shdr->init();

	shdr->setShaderCode(ShaderStages::VERTEX_STAGE, (char*)vs.c_str());
	shdr->setShaderCode(ShaderStages::GEOMETRY_STAGE, (char*)gs.c_str());
	shdr->setShaderCode(ShaderStages::FRAGMENT_STAGE, (char*)fs.c_str());

	if (!shdr->buildShader()) {
		gConsole->print("shader failed to build\n");
		assert(0 && "shader failed to build");
	}

	vpLocation = shdr->getShaderUniform("viewProjMatrix");
	matLocation = shdr->getShaderUniform("worldMat");
	selectedLoc = shdr->getShaderUniform("selectedColor");*/

	// force update window to correct size
	// @todo load from config
	Input::getInput()->window->setWindowSize(1280, 720);
}

bool Game::isRunning() const {
	return running;
}

bool Game::updateAndRender(float dt) {

	__try {

		core->update(dt);
		if (core->hadGraphicsReset()) {
			return false;
		}
		tickFPS(dt);
		running = core->isRunning();

		Input* in = Input::getInput();
		if (in->sizeChange) {
			int w = 0, h = 0;
			in->getWindowSize(w, h);
			if (w != 0 && h != 0) {
				*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(w), float(h), 0.0001f, 100.0f);
				*(glm::mat4*)(cam->getOrthoMatrix()) = glm::ortho(0.0f, float(w), float(h), 0.0f);
				gBuffer->resize(w, h);
				gBuffer->setWindowSize(w, h);
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

	} __except (EXCEPTION_EXECUTE_HANDLER) {
		gConsole->print("Serious error has occured!\n");
		return false;
	}

	return true;
}

void Game::render() {
	glm::mat4 vp = *(glm::mat4*)cam->getPerspectiveMatrix() * *(glm::mat4*)cam->getViewMatrix();

	core->render(vp);

	gBuffer->bind();
	gBuffer->clear();

	if (currentScene != nullptr) {
		currentScene->render(vp);
	}

	//shdr->useShader();
	//shdr->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, &vp);
	//shdr->bindData(selectedLoc, UniformDataType::UNI_FLOAT3, &glm::vec3(1.0f));

	if (gstate == GameState::eGameStage_MainMenu) {
		// todo figure out menu stuff
	}

	gBuffer->resolveToScreen();
	gRenderEngine->bindDefaultFrameBuffer();

	guiPanel->render(*(glm::mat4*)cam->getOrthoMatrix());

	core->swap();
}

void Game::updateMenu(float dt) {
	Input* in = Input::getInput();

	if (in->releasedThisFrame(KeyBindings[KEYBIND_FORWARD], false)) {
		// todo figure out menu stuff
	}

	if (in->releasedThisFrame(KeyBindings[KEYBIND_BACKWARD], false)) {
		// todo figure out menu stuff
	}

	if (in->releasedThisFrame(KeyBindings[KEYBIND_ENTER], false)) {
		// todo figure out menu stuff
	}
}

void Game::updateEditor(float dt) {
	camInput.update(dt);
	currentScene = editAccess.getCurrentScene();

	if (core->currentEditorStatus == EditorStatus::HIDDEN || core->currentEditorStatus == EditorStatus::STOPPED) {
		gstate = GameState::eGameStage_MainMenu;
		//sceneThisFrame = &currentGameScene;
	}
}

void Game::updateGame(float dt) {
	camInput.update(dt);

	if (currentScene != nullptr) {
		currentScene->update(dt);
	}

}

void Game::updateLoading(float dt) {
	// show some loading screen stuff and display loading status or something similar
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