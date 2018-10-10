#ifndef SCENE_HPP
#define SCENE_HPP

/// Internal Includes
#include "../GameObject.hpp"

/// External Includes

/// Std Includes
#include <vector>

class Scene {

public:

	Scene();
	virtual ~Scene();

	void addGameObject(GameObject* go);

	void update(float dt);

	void render();

private:
	
	std::vector<GameObject*> gameObjects;

};

#endif