#ifndef SCENE_HPP
#define SCENE_HPP

#include "GameObject.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

#include <vector>

class Scene {

public:

	void init();

	void release();

	uint32_t getGameObjectCount();

private:

	// @todo not use vector
	std::vector<GameObject*> gameObjects;

	std::vector<RenderComponent*> renderComponents;
	std::vector<TransformComponent*> transformComponents;

};

#endif