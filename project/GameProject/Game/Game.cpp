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

Game::Game() : player(nullptr), running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu), gameStarted(false), enterLeaveMenuState(false) {}

Game::~Game() {
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

	if ( !AssetManager::getAssetManager()->masterFile.loadMaster("Data/master.mst") ) {
		core->startEditor();
	}

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

	//@EndTemporary

	nrMemuItems = 4;
	currentMenuItem = 0;

	menuPosX = 1920 - 250;
}

bool Game::isRunning() const {
	return running;
}

void Game::update(float dt) {
	core->update(dt);
	tickFPS(dt);
	running = core->isRunning();

	int fontSize = AssetManager::getAssetManager()->getBasicFont()->getFontSize();

	renderedMenuItems = 0;
	menuPosY = 1080 - 200;
	stride = fontSize + 10;

	//@Temporary
	Input* in = Input::getInput();

	if ( in->sizeChange ) {
		int w = 0, h = 0;
		in->getWindowSize(w, h);
		*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(w), float(h), 0.0001f, 100.0f);
	}

	if ( in->releasedThisFrame(KeyBind{ 1,0,0 }, false) && gameStarted ) {
		enterLeaveMenuState = true;
		toggleMenuTarget();
	}

	if ( enterLeaveMenuState ) {
		if ( menuPosX == menuXTarget ) {
			enterLeaveMenuState = false;
			toggleMenu();
		}

		if ( menuXTarget > menuPosX ) menuPosX += 10;
		else menuPosX -= 10;
	}

	// update gameStuffz
	if ( gstate == GameState::eGameState_PlayMode && (!enterLeaveMenuState) ) {
		camInput.update(dt);
	}

	else if ( gstate == GameState::eGameStage_MainMenu ) {
		KeyBind kb;
		kb.code = 72;
		kb.mod = 0;
		kb.mouse = 0;

		bool consoleActive = Input::getInput()->consoleIsActive();

		if ( in->releasedThisFrame(kb) && !consoleActive ) {
			//std::cout << "UP Pressed\n";
			//saveGame();
			handleMenuEvent(-1);
		}

		kb.code = 80;

		if ( in->releasedThisFrame(kb) && !consoleActive ) {
			//std::cout << "Down Pressed\n";
			handleMenuEvent(+1);
			//loadGame();
		}
		kb.code = 28;

		if ( in->releasedThisFrame(kb) && !consoleActive ) {
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
	for ( size_t i = 0; i < gameObjects.size(); i++ ) {
		shObj->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &gameObjects[i].getMatrix());
		IMesh* m = gameObjects[i].getMesh();
		m->bind();
		m->render();
	}

	IRenderEngine* re = core->getRenderEngine();

	if ( gstate == GameState::eGameStage_MainMenu || menuPosX != menuXTarget ) {
		textShObj->useShader();


		re->setBlending(true);

		newGameIndex = renderMenuItem("New Game", 8);
		if ( gameStarted )
			saveGameIndex = renderMenuItem("Save Game", 9);
		loadGameIndex = renderMenuItem("Load Game", 9);
		if ( core->editorAvaible() )
			editorIndex = renderMenuItem("Editor", 6);
		quitGameIndex = renderMenuItem("Quit", 4);

		re->setBlending(false);

		nrMemuItems = renderedMenuItems;
	}

	if ( ffps > 40 )
		textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &glm::vec3(0, 1, 0));
	else if ( ffps > 20 )
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

	if ( mapFile.is_open() ) {
		std::string line = "";

		std::getline(mapFile, line);
		objectCount = std::stoi(line);

		for ( int i = 0; i < objectCount; i++ ) {
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
	enterLeaveMenuState = true;
	menuXTarget = 1950;

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

int Game::renderMenuItem(char * text, int length) {
	glm::mat4 vp = *(glm::mat4*)cam->getOrthoMatrix();

	glm::vec3 color = glm::vec3(1, 1, 1);

	int index = renderedMenuItems;

	if ( index == currentMenuItem )
		color = glm::vec3(1, 0, 0);

	textShObj->bindData(orthoLocation, UniformDataType::UNI_MATRIX4X4, &vp);
	textShObj->bindData(textLocation, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &color);
	t->setText(text, length, (float)menuPosX, (float)menuPosY, 1.0);
	menuPosY += stride;

	t->render(textShObj, textureLocation);

	renderedMenuItems++;

	return index;
}

void Game::handleMenuEvent(int advance) {
	currentMenuItem += advance;

	if ( currentMenuItem < 0 ) currentMenuItem += nrMemuItems;
	if ( currentMenuItem >= nrMemuItems ) currentMenuItem -= nrMemuItems;
}

void Game::handleMenuEnter() {
	int choice = currentMenuItem;

	if ( choice == newGameIndex ) {
		newGame();
	}

	if ( choice == saveGameIndex ) {
		saveGame();
	}

	if ( choice == loadGameIndex ) {
		loadGame();
	}

	if ( choice == editorIndex ) {
		core->startEditor();
	}

	if ( choice == quitGameIndex && !core->isInEditor() ) {
		running = false;
	} else if ( choice == quitGameIndex && core->isInEditor() ) {
		core->getConsole()->print("Editor mode enabled cannot quit using game menu");
	}
}

void Game::toggleMenu() {
	if ( gstate == GameState::eGameStage_MainMenu && gameStarted ) {
		gstate = GameState::eGameState_PlayMode;
	} else {
		gstate = GameState::eGameStage_MainMenu;
		currentMenuItem = 0;
	}
}

void Game::toggleMenuTarget() {
	if ( gstate == GameState::eGameStage_MainMenu )
		menuXTarget = 1950;
	else
		menuXTarget = 1920 - 250;
}

void Game::tickFPS(float dt) {
	fps++;
	timepass += dt;

	if ( timepass > 1.0f ) {
		core->setFPS(fps);
		ffps = fps;
		fps = 0;
		timepass -= 1.0f;
	}
}