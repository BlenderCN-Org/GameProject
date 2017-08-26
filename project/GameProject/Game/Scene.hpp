#ifndef SCENE_HPP
#define SCENE_HPP

#include "GameObject.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

#include "../Core/Assets/DataObjects.hpp"

#include <vector>

class Scene {
public:

	void init();

	void release();

	void setSceneDataObj(SceneDataObject* dataObj);

	glm::vec4 getSkyColor();

	uint32_t getGameObjectCount();
	uint32_t getSceneID();

private:

	SceneDataObject* sceneDataObj;

	// @todo not use vector
	std::vector<GameObject*> gameObjects;

	std::vector<RenderComponent*> renderComponents;
	std::vector<TransformComponent*> transformComponents;
};

#endif