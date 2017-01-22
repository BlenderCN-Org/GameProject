#include "Game.hpp"

#include "../Core/Input/Input.hpp"

#include <glm\gtc\matrix_transform.hpp>

Game::Game() : player(nullptr), running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu) {}

Game::~Game() {
	delete player;
	delete cam;

	core->release();
}

void Game::init() {
	core = new Core();
	core->init();

	cam = new Camera();

	camInput.init((glm::mat4*)cam->getViewMatrix());
	*(glm::mat4*)(cam->getPerspectiveMatrix()) = glm::perspectiveFov(glm::radians(45.0f), float(1280), float(720), 0.0001f, 100.0f);
	*(glm::mat4*)(cam->getOrthoMatrix()) = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);

	player = new GameObject();

	//if ( core->getMemoryManager()->poolExists(0) ) {
	//	printf("Pool exists\n");
	//}
	//if ( !core->getMemoryManager()->poolExists(1) ) {
	//	printf("Pool does not exists\n");
	//}

}

bool Game::isRunning() const {
	return running;
}

void Game::update(float dt) {
	core->update(dt);
	camInput.update(dt);
	tickFPS(dt);
	running = core->isRunning();

	// update gameStuffz
	if ( gstate == GameState::eGameState_PlayMode ) {

	}

	Input* in = Input::getInput();

	KeyBind kb;
	kb.code = 2;
	kb.mod = 0;
	kb.mouse = 0;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "1 Pressed\n";
		saveGame();
	}

	kb.code = 3;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "2 Pressed\n";
		loadGame();
	}

}

void Game::render() {

	glm::mat4 vp = *(glm::mat4*)cam->getPerspectiveMatrix() * *(glm::mat4*)cam->getViewMatrix();

	core->render(vp);
}

void Game::newGame() {}

void Game::saveGame() {
	std::cout << "Saving Game\n";

	core->getMemoryManager()->saveHeap();

}

void Game::loadGame() {
	std::cout << "Loading Game\n";

	core->getMemoryManager()->loadHeap();

}

void Game::tickFPS(float dt) {
	fps++;
	timepass += dt;

	if ( timepass > 1.0f ) {
		core->setFPS(fps);
		fps = 0;
		timepass -= 1.0f;
	}
}