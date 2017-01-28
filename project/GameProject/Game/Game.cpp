#include "Game.hpp"

#include "../Core/Input/Input.hpp"

#include <glm\gtc\matrix_transform.hpp>

#include <AssetLib\AssetLib.hpp>

//@Temporary

struct loadedData
{
	std::string meshName;
	int x, y, z;
};

struct meshStruct {
	IMesh* mesh;
	glm::mat4 matrix;
};

std::vector<meshStruct> tempMeshes;

meshStruct createMeshStruct(Core* core, std::string meshName, float x, float y, float z) {
	IRenderEngine* re = core->getRenderEngine();

	meshStruct ms{};

	ms.mesh = re->createMesh();
	ms.mesh->init(MeshPrimitiveType::TRIANGLE);

	uint32_t size = 0;
	void* data = AssetLib::loadWavefrontOBJ(meshName.c_str(), size);

	ms.mesh->setMeshData(data, size, MeshDataLayout::VERT_UV);
	delete data;

	ms.matrix = glm::mat4();
	ms.matrix[0][3] = x;
	ms.matrix[1][3] = y;
	ms.matrix[2][3] = z;

	return ms;
}

//@EndTemporary

Game::Game() : player(nullptr), running(true), core(nullptr), timepass(0.0f), fps(0), gstate(GameState::eGameStage_MainMenu) {}

Game::~Game() {
	delete player;
	delete cam;

	for ( size_t i = 0; i < tempMeshes.size(); i++ ) {
		tempMeshes[i].mesh->release();
	}

	t->release();

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
	t->setFont(core->getAssetManager()->getBasicFont());

	//@EndTemporary

	nrMemuItems = 4;
	currentMenuItem = 0;

}

bool Game::isRunning() const {
	return running;
}

void Game::update(float dt) {
	core->update(dt);
	camInput.update(dt);
	tickFPS(dt);
	running = core->isRunning();

	menuItemCounter = 0;


	// update gameStuffz
	if ( gstate == GameState::eGameState_PlayMode ) {

	}

	//@Temporary
	Input* in = Input::getInput();

	KeyBind kb;
	kb.code = 72;
	kb.mod = 0;
	kb.mouse = 0;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "UP Pressed\n";
		//saveGame();
		handleMenuEvent(-1);
	}

	kb.code = 80;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "Down Pressed\n";
		handleMenuEvent(+1);
		//loadGame();
	}
	kb.code = 28;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "Enter \n";
		//newGame();
		handleMenuEnter();
	}

	//@EndTemporary

}

void Game::render() {

	glm::mat4 vp = *(glm::mat4*)cam->getPerspectiveMatrix() * *(glm::mat4*)cam->getViewMatrix();

	core->render(vp);

	shObj->useShader();
	shObj->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, &vp);

	//@Temporary
	for ( size_t i = 0; i < tempMeshes.size(); i++ ) {

		shObj->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &tempMeshes[i].matrix);
		tempMeshes[i].mesh->bind();
		tempMeshes[i].mesh->render();
	}

	textShObj->useShader();

	IRenderEngine* re = core->getRenderEngine();

	re->setBlending(true);

	newGameIndex = renderMenuItem("New Game", 8, 10, 10);
	saveGameIndex = renderMenuItem("Save Game", 9, 10, 30);
	loadGameIndex = renderMenuItem("Load Game", 9, 10, 50);
	quitGameIndex = renderMenuItem("Quit", 4, 10, 70);

	re->setBlending(false);
	//@EndTemporary

	core->swap();

}

void Game::newGame() {

	for ( size_t i = 0; i < tempMeshes.size(); i++ ) {
		tempMeshes[i].mesh->release();
	}

	tempMeshes.clear();

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

			tempMeshes.push_back(createMeshStruct(core, meshName, x, y, z));

		}

		int b = 0;

	}

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

int Game::renderMenuItem(char * text, int length, int x, int y) {

	glm::mat4 vp = *(glm::mat4*)cam->getOrthoMatrix();

	glm::vec3 color = glm::vec3(1, 1, 1);

	int index = menuItemCounter;

	if ( index == currentMenuItem )
		color = glm::vec3(1, 0, 0);

	textShObj->bindData(orthoLocation, UniformDataType::UNI_MATRIX4X4, &vp);
	textShObj->bindData(textLocation, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	textShObj->bindData(colorLocation, UniformDataType::UNI_FLOAT3, &color);
	t->setText(text, length, x, y, 1.0);
	t->render(textShObj, textureLocation);

	

	menuItemCounter++;

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
	if ( choice == quitGameIndex ) {
		running = false;
	}

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