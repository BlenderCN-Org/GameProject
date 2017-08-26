#include "EditorAccess.hpp"
#include "Game.hpp"

#include "../Core/CoreGlobals.hpp"
#include "../Core/AssetManager.hpp"

EditorAccess::EditorAccess() {
	startupScene.init();
	selectedScene = nullptr;
	game = nullptr;
}

EditorAccess::~EditorAccess() {
	if (selectedScene != nullptr) {
		delete selectedScene;
		selectedScene = nullptr;
	}
}

void EditorAccess::setActiveScene(uint32_t sceneFormID) {
	if (selectedScene != nullptr) {
		delete selectedScene;
		selectedScene = nullptr;
	}

	Entry* e = gAssetManager->getEntry(sceneFormID);
	if (e) {
		SceneDataObject* sDataObj = (SceneDataObject*)gAssetManager->getConverter()->asSceneData(e->data);
		if (sDataObj) {
			selectedScene = new Scene();
			selectedScene->setSceneDataObj(sDataObj);
		}
	}
}

void EditorAccess::placeObject(uint32_t objectFormID) {
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