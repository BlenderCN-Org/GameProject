#include "../Core/CoreGlobals.hpp"
#include "../Core/AssetManager.hpp"
#include "../Core/System/Console.hpp"

#include "Scene.hpp"

#include <string>

#include <glm/gtc/matrix_transform.hpp>

std::string readShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		gConsole->print("Could not open file %s", filePath);
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

GameObject* createGo2(AssetManager* assetManager, std::string meshName, float x, float y, float z) {
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

Scene::Scene() {
	shaders = nullptr;
	cells = nullptr;
}

void Scene::init(uint32_t formId) {

	cells = nullptr;
	nCellsX = 0U;
	nCellsY = 0U;

	Entry* e = gAssetManager->getEntry(formId);

	if (e) {
		// currently does nothing having a fake scene
	}

	{

		shaderCount = 1;

		shaders = gRenderEngine->createShaderObject();
		shaders->init();

		std::string vs = readShader("data/shaders/default.vs.glsl");
		std::string gs = readShader("data/shaders/default.gs.glsl");
		std::string fs = readShader("data/shaders/default.fs.glsl");

		shaders->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
		shaders->setShaderCode(ShaderStages::GEOMETRY_STAGE, gs.c_str());
		shaders->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

		if (!shaders->buildShader()) {
			gConsole->print("Failed to build shader!\n");
			assert(0 && "Failed to build shader!");
		}

		vpLocation = shaders->getShaderUniform("viewProjMatrix");
		matLocation = shaders->getShaderUniform("worldMat");
		selectedLoc = shaders->getShaderUniform("selectedColor");

		nCellsX = 4;
		nCellsY = 4;
		cells = new Cell*[nCellsX];
		for (size_t x = 0; x < nCellsX; x++) {
			cells[x] = new Cell[nCellsY];

			cells[x][0].position = glm::vec3(0, 0, 0);
			cells[x][0].size = CELL_SIZE;
			cells[x][0].gameObjectCount = 1;
			cells[x][0].gameObjects = new GameObject*[cells[x][0].gameObjectCount];
			cells[x][0].gameObjects[0] = createGo2(gAssetManager, "data/meshes/test.mesh", CELL_SIZE * x, 0, 0);

			cells[x][1].position = glm::vec3(0, 0, 0);
			cells[x][1].size = CELL_SIZE;
			cells[x][1].gameObjectCount = 1;
			cells[x][1].gameObjects = new GameObject*[cells[x][1].gameObjectCount];
			cells[x][1].gameObjects[0] = createGo2(gAssetManager, "data/meshes/test.mesh", CELL_SIZE * x, 0, CELL_SIZE);

			cells[x][2].position = glm::vec3(0, 0, 0);
			cells[x][2].size = CELL_SIZE;
			cells[x][2].gameObjectCount = 1;
			cells[x][2].gameObjects = new GameObject*[cells[x][2].gameObjectCount];
			cells[x][2].gameObjects[0] = createGo2(gAssetManager, "data/meshes/test.mesh", CELL_SIZE * x, 0, CELL_SIZE * 2);

			cells[x][3].position = glm::vec3(0, 0, 0);
			cells[x][3].size = CELL_SIZE;
			cells[x][3].gameObjectCount = 3;
			cells[x][3].gameObjects = new GameObject*[cells[x][3].gameObjectCount];
			cells[x][3].gameObjects[0] = createGo2(gAssetManager, "data/meshes/tile_4x4.mesh", CELL_SIZE * x, 0, CELL_SIZE*3);
			cells[x][3].gameObjects[1] = createGo2(gAssetManager, "data/meshes/tile_4x4.mesh", CELL_SIZE * x - 4, 0, CELL_SIZE * 3 - 4);
			cells[x][3].gameObjects[2] = createGo2(gAssetManager, "data/meshes/tile_2x2.mesh", CELL_SIZE * x - 4, 0, CELL_SIZE * 3 - 7);
		}
	}
}

void Scene::release() {
	shaders->release();
	if (cells) {
		for (size_t x = 0; x < nCellsX; x++) {
			for (size_t y = 0; y < nCellsY; y++) {
				for (size_t i = 0; i < cells[x][y].gameObjectCount; i++) {
					delete cells[x][y].gameObjects[i];
				}
				delete[] cells[x][y].gameObjects;
			}
			delete[] cells[x];
			cells[x] = nullptr;
		}
		delete[] cells;
		cells = nullptr;
	}

	nCellsX = 0U;
	nCellsY = 0U;
}

void Scene::update(float dt) {

	skyColor = glm::vec4(0.5F, 0.0F, 0.5F, 0.0F);
	gRenderEngine->setClearColor(skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	/*if (gConsole->shaderCrash == true) {
		gConsole->shaderCrash = false;

		std::string vs = readShader("data/shaders/default.vs.glsl");
		std::string gs = readShader("data/shaders/default.gs.glsl");
		std::string fs = readShader("data/shaders/default.fs.crash.glsl");

		shaders->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
		shaders->setShaderCode(ShaderStages::GEOMETRY_STAGE, gs.c_str());
		shaders->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

		if (!shaders->buildShader()) {
			gConsole->print("Failed to build shader!\n");
		}

		vpLocation = shaders->getShaderUniform("viewProjMatrix");
		matLocation = shaders->getShaderUniform("worldMat");
		selectedLoc = shaders->getShaderUniform("selectedColor");

	}*/
}

void Scene::render(const glm::mat4 &vp) {

	if (shaders && cells) {
		shaders->useShader();
		shaders->bindData(vpLocation, UniformDataType::UNI_MATRIX4X4, (void*)&vp);
		shaders->bindData(selectedLoc, UniformDataType::UNI_FLOAT3, &glm::vec3(1.0f));

		for (size_t i = 0; i < nCellsX; i++) {
			for (size_t y = 0; y < nCellsY; y++) {
				for (size_t g = 0; g < cells[i][y].gameObjectCount; g++) {
					RenderComponent* rComp = (RenderComponent*)cells[i][y].gameObjects[g]->getComponent<RenderComponent>();
					TransformComponent* tc = (TransformComponent*)cells[i][y].gameObjects[g]->getComponent<TransformComponent>();
					uint32_t inst = rComp->getInstanceId();

					IMesh * mesh = gAssetManager->getMeshFromInstanceId(inst);
					if (mesh) {
						glm::mat4 mdl = tc->getModelMatrix();
						shaders->bindData(matLocation, UniformDataType::UNI_MATRIX4X4, &mdl);
						mesh->bind();
						mesh->render();
					}
				}
			}
		}
	}
}
