#include "Scene.hpp"

void Scene::init() {

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

uint32_t Scene::getGameObjectCount() {
	return (uint32_t)gameObjects.size();
}