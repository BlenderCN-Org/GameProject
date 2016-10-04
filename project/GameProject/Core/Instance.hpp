#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <glm\glm.hpp>
#include <RenderEngine\IMesh.hpp>
#include <RenderEngine\IMaterial.hpp>

class Instance
{

public:

private:

	uint64_t instanceID;
	IMesh* mesh;
	IMaterial* material;

	uint16_t instanceCount;
	uint16_t instanceSize;

	glm::mat4* matrices;

};

#endif