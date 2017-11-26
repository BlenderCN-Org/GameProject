#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "../Engine/Graphics/Mesh/StaticMesh.hpp"

#include <glm/glm.hpp>

class GameObject {

public:

	GameObject();
	virtual ~GameObject();



private:

	glm::mat4 worldMatrix;
	Engine::Graphics::Mesh::StaticMesh* mesh;

};

#endif