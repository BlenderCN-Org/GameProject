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

	vpLocation = shObj->getShaderUniform("viewProjMatrix");
	matLocation = shObj->getShaderUniform("worldMat");
	//@EndTemporary

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
	
	//@Temporary
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
	kb.code = 4;

	if ( in->releasedThisFrame(kb) ) {
		std::cout << "3 Pressed\n";
		newGame();
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

void Game::tickFPS(float dt) {
	fps++;
	timepass += dt;

	if ( timepass > 1.0f ) {
		core->setFPS(fps);
		fps = 0;
		timepass -= 1.0f;
	}
}