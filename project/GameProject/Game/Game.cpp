#include "Game.hpp"

#include "../Core/Input/Input.hpp"

#include <glm\gtc\matrix_transform.hpp>

#include <AssetLib\AssetLib.hpp>

//@Temporary

GameObject createMeshStruct(Core* core, std::string meshName, float x, float y, float z) {
	IRenderEngine* re = core->getRenderEngine();

	GameObject go;
	IMesh* mesh = AssetManager::getAssetManager()->getMesh((char*)meshName.c_str());

	go.init();

	go.setMesh(mesh);
	glm::mat4 mat = glm::mat4();
	mat[0][3] = x;
	mat[1][3] = y;
	mat[2][3] = z;
	go.setMatrix(mat);

	return go;
}

//@EndTemporary

Game::Game() : player(nullptr), running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu), gameStarted(false) {}

Game::~Game() {
	delete mainMenu;

	delete player;
	delete cam;

	delete t;

	delete core;
}

void Game::init() {
	core = new Core();
	core->init();

	cam = new Camera();

	camInput.init((glm::mat4*)cam->getViewMatrix());
	*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);
	*(glm::mat4*)(cam->getOrthoMatrix()) = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);

	player = new GameObject();

	if (!AssetManager::getAssetManager()->masterFile.loadMaster("Data/master.mst")) {
		core->getConsole()->print("Could not load masterfile\n");
	}
	//if ( !AssetManager::getAssetManager()->masterFile.loadMaster("Data/master.mst") ) {
	core->startEditor();
	//}

	//@Temporary
	shObj = core->getShaderObject();
	textShObj = core->getTextShaderObject();

	vpLocation = shObj->getShaderUniform("viewProjMatrix");
	matLocation = shObj->getShaderUniform("worldMat");

	orthoLocation = textShObj->getShaderUniform("viewProjMatrix");
	textLocation = textShObj->getShaderUniform("worldMat");
	textureLocation = textShObj->getShaderUniform("text");
	colorLocation = textShObj->getShaderUniform("textColor");

	t = new Text();
	t->init(core->getRenderEngine());
	t->setFont(AssetManager::getAssetManager()->getBasicFont());

	if (AssetManager::getAssetManager()->loadMenu(&mainMenu, "MainMenu")) {
		core->getConsole()->print("Menu loaded!\n");
	}

	//@EndTemporary

}

bool Game::isRunning() const {
	return running;
}

void Game::update(float dt) {
	core->update(dt);
	tickFPS(dt);
	running = core->isRunning();

	// stuff that we only do while not being in the editor
	if (!core->isInEditor()) {
		mainMenu->update();
	}

	Input* in = Input::getInput();
	if (in->sizeChange) {
		int w = 0, h = 0;
		in->getWindowSize(w, h);
		*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(w), float(h), 0.0001f, 100.0f);
	}

	// update gameStuffz
	if (gstate == GameState::eGameState_PlayMode) {
		camInput.update(dt);
	}

	else if (gstate == GameState::eGameStage_MainMenu) {
		KeyBind kb;
		kb.code = 72;
		kb.mod = 0;
		kb.mouse = 0;

		bool consoleActive = Input::getInput()->consoleIsActive();

		if (in->releasedThisFrame(kb) && !consoleActive) {
			//std::cout << "UP Pressed\n";
			//saveGame();
			handleMenuEvent(-1);
		}

		kb.code = 80;

		if (in->releasedThisFrame(kb) && !consoleActive) {
			//std::cout << "Down Pressed\n";
			handleMenuEvent(+1);
			//loadGame();
		}
		kb.code = 28;

		if (in->releasedThisFrame(kb) && !consoleActive) {
			//std::cout << "Enter \n";
			//newGame();
			handleMenuEnter();
		}
	}
	//@EndTemporary
}

void Game::render() {
	glm::mat4 vp = *(glm::mat4*)cam->getPerspectiveMatrix() * *(glm::mat4*)cam->getViewMatrix();

	core->render(vp);

	shObj->useShader();
	shObj->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, &vp);

	//@Temporary
	for (size_t i = 0; i < gameObjects.size(); i++) {
		shObj->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &gameObjects[i].getMatrix());
		IMesh* m = gameObjects[i].getMesh();
		m->bind();
		m->render();
	}

	// stuff that we only do while not being in the editor
	if (!core->isInEditor()) {
		mainMenu->render();
	}

	IRenderEngine* re = core->getRenderEngine();

	if (ffps > 60)
		textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(0, 1, 0));
	else if (ffps > 30)
		textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(255.0f / 255.0f, 165.0f / 255.0f, 0));
	else
		textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(1, 0, 0));

	std::string fpsString = std::to_string(ffps) + " FPS";

	t->setText((char*)fpsString.c_str(), fpsString.size(), 10, 10, 1.0);
	re->setBlending(true);
	t->render(textShObj, textureLocation);
	re->setBlending(false);


	//@EndTemporary
	core->renderConsole();
	core->swap();
}

void Game::newGame() {
	gameObjects.clear();

	//@Temporary
	mapFile.open("data/map.world");

	int objectCount = 0;

	if (mapFile.is_open()) {
		std::string line = "";

		std::getline(mapFile, line);
		objectCount = std::stoi(line);

		for (int i = 0; i < objectCount; i++) {
			std::string meshName = "";
			float x = 0;
			float y = 0;
			float z = 0;

			std::getline(mapFile, meshName);

			std::getline(mapFile, line);
			x = std::stof(line);
			std::getline(mapFile, line);
			y = std::stof(line);
			std::getline(mapFile, line);
			z = std::stof(line);

			gameObjects.push_back(createMeshStruct(core, meshName, x, y, z));
		}

		int b = 0;
	}

	gameStarted = true;
	//mainMenu->enterLeaveMenuState = true;
	//mainMenu->menuXTarget = 1950;

	camInput.setCam(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1));

	mapFile.close();
	//@EndTermporary
}

void Game::saveGame() {
	std::cout << "Saving Game\n";

	core->getMemoryManager()->saveHeap();
}

void Game::loadGame() {
	std::cout << "Loading Game\n";

	core->getMemoryManager()->loadHeap();
}


void Game::handleMenuEvent(int advance) {
	//mainMenu->currentMenuItem += advance;
	//
	//if (mainMenu->currentMenuItem < 0) mainMenu->currentMenuItem += mainMenu->nrMemuItems;
	//if (mainMenu->currentMenuItem >= mainMenu->nrMemuItems) mainMenu->currentMenuItem -= mainMenu->nrMemuItems;
}

void Game::handleMenuEnter() {
	//int choice = mainMenu->currentMenuItem;
	//
	//if (choice == mainMenu->newGameIndex) {
	//	newGame();
	//}
	//
	//if (choice == mainMenu->saveGameIndex) {
	//	saveGame();
	//}
	//
	//if (choice == mainMenu->loadGameIndex) {
	//	loadGame();
	//}
	//
	//if (choice == mainMenu->editorIndex) {
	//	core->startEditor();
	//}
	//
	//if (choice == mainMenu->quitGameIndex && !core->isInEditor()) {
	//	running = false;
	//}
	//else if (choice == mainMenu->quitGameIndex && core->isInEditor()) {
	//	core->getConsole()->print("Editor mode enabled cannot quit using game menu");
	//}
}

void Game::toggleMenu() {
	//if (gstate == GameState::eGameStage_MainMenu && gameStarted) {
	//	gstate = GameState::eGameState_PlayMode;
	//}
	//else {
	//	gstate = GameState::eGameStage_MainMenu;
	//	mainMenu->currentMenuItem = 0;
	//}
}

void Game::toggleMenuTarget() {
	//if (gstate == GameState::eGameStage_MainMenu)
	//	mainMenu->menuXTarget = 1950;
	//else
	//	mainMenu->menuXTarget = 1920 - 250;
}

void Game::tickFPS(float dt) {
	fps++;
	timepass += dt;

	if (timepass > 1.0f) {
		core->setFPS(fps);
		ffps = fps;
		fps = 0;
		timepass -= 1.0f;
	}
}