#include "EditorAccess.hpp"
#include "Game.hpp"

#include "../Core/CoreGlobals.hpp"
#include "../Core/AssetManager.hpp"

// @temp
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

EditorAccess::EditorAccess() {
	//startupScene.init();
	//selectedScene = nullptr;
	//game = nullptr;
}

EditorAccess::~EditorAccess() {
	//if (selectedScene != nullptr) {

	//	for (size_t i = 0; i < selectedScene->gameObjects.size(); i++) {
	//		delete selectedScene->gameObjects[i];
	//	}

	//	delete selectedScene;
	//	selectedScene = nullptr;
	//}
}

void EditorAccess::setActiveScene(uint32_t sceneFormID) {
	//if (selectedScene != nullptr) {

	//	for (size_t i = 0; i < selectedScene->gameObjects.size(); i++) {
	//		delete selectedScene->gameObjects[i];
	//	}

	//	delete selectedScene;
	//	selectedScene = nullptr;
	//}

	//Entry* e = gAssetManager->getEntry(sceneFormID);
	//if (e) {
	//	SceneDataObject* sDataObj = (SceneDataObject*)gAssetManager->getConverter()->asSceneData(e->data);
	//	if (sDataObj) {
	//		selectedScene = new Scene();
	//		selectedScene->setSceneDataObj(sDataObj);
	//	}
	//}
}

void EditorAccess::placeObject(uint32_t objectFormID) {

	//Entry* e = gAssetManager->getEntry(objectFormID);

	//if (e->data->getType() == IGameDataObject::STATICOBJECT && selectedScene != nullptr) {
	//	StaticObjectSaveData* data = gAssetManager->getConverter()->asStaticObjectData(e->data)->getStaticObjectData();
	//	GameObject* go = createGo(gAssetManager, data->meshFile, 0, 0, 0);
	//	selectedScene->gameObjects.push_back(go);
	//}
}

void EditorAccess::removeObject(uint32_t objectRefID) {
}

void EditorAccess::selectObject(uint32_t objectRefID) {
}

void EditorAccess::setGameInstance(Game * g) {
	game = g;
}

Scene * EditorAccess::getCurrentScene() {
	if (selectedScene == nullptr) {
		return &startupScene;
	}
	return selectedScene;
}