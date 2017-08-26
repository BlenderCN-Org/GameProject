#include "Scene.hpp"

void Scene::init() {
	sceneDataObj = nullptr;
}

void Scene::release() {
	std::vector<GameObject*>::const_iterator git = gameObjects.begin();
	std::vector<GameObject*>::const_iterator geit = gameObjects.end();

	for (git; git != geit; git++) {
		if (*git) {
			delete *git;
		}
	}

	std::vector<RenderComponent*>::const_iterator rit = renderComponents.begin();
	std::vector<RenderComponent*>::const_iterator reit = renderComponents.end();

	for (rit; rit != reit; rit++) {
		if (*rit) {
			delete *rit;
		}
	}

	std::vector<TransformComponent*>::const_iterator tit = transformComponents.begin();
	std::vector<TransformComponent*>::const_iterator teit = transformComponents.end();

	for (tit; tit != teit; tit++) {
		if (*tit) {
			delete *tit;
		}
	}

	delete this;
}

void Scene::setSceneDataObj(SceneDataObject* dataObj) {
	sceneDataObj = dataObj;
}

glm::vec4 Scene::getSkyColor() {
	if (sceneDataObj) {
		SceneSaveData* sdata = sceneDataObj->getSceneData();
		return glm::vec4(sdata->skyColor[0], sdata->skyColor[1], sdata->skyColor[2], sdata->skyColor[3]);
	}
	return glm::vec4(0, 0, 0, 1);
}

uint32_t Scene::getGameObjectCount() {
	return (uint32_t)gameObjects.size();
}

uint32_t Scene::getSceneID() {
	return 0;
}