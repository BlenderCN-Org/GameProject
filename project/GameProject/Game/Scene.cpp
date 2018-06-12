#include "Scene.hpp"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::addGameObject(GameObject* go) {
	gameObjects.push_back(go);
}

void Scene::update(float dt) {

}

void Scene::render() {

	std::vector<GameObject*>::const_iterator it = gameObjects.begin();
	std::vector<GameObject*>::const_iterator eit = gameObjects.end();

	for (it; it != eit; it++) {
		//(*it)
	}

}