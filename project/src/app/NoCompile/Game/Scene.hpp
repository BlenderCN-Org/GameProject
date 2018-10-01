#ifndef SCENE_HPP
#define SCENE_HPP

#include <RenderEngine/IRenderEngine.hpp>

#include "GameObject.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/TransformComponent.hpp"

#include "../Core/Assets/DataObjects.hpp"

#include <vector>

#define CELL_SIZE 16.0F

struct Cell {

	glm::vec3 position;
	float size;

	uint32_t gameObjectCount;
	GameObject** gameObjects;
};

struct UniformList {
	uint32_t numUniforms;
	int32_t* uniforms;
};

class Scene {
public:
	Scene();

	void init(uint32_t formId);

	void release();

	glm::vec4 skyColor;
	glm::vec4 fogNear;
	glm::vec4 fogFar;

	uint32_t nCellsX;
	uint32_t nCellsY;
	Cell** cells;

	uint32_t shaderCount;
	IShaderObject* shaders;
	UniformList* uniforms;

	int32_t vpLocation;
	int32_t matLocation;
	int32_t selectedLoc;

	void update(float dt);
	void render(const glm::mat4 &vp);

private:

};

#endif