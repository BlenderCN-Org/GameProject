#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

/// Internal Includes
#include "../Engine/Graphics/Mesh/Mesh.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

class GameObject {

public:

	GameObject();
	virtual ~GameObject();



private:

	glm::mat4 worldMatrix;
	Engine::Graphics::Mesh::CMesh* mesh;

};

#endif