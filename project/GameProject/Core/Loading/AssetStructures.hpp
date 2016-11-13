#ifndef ASSETSTRUCTURES_HPP
#define ASSETSTRUCTURES_HPP

#include <cstdint>

#include <glm\glm.hpp>
// todo see what tags are needed and define them

enum class HeaderTags : uint32_t
{
	eTagModel = 'MDL ', // tag for model file/data
	eTagTexture = 'TEX ', // tag for texture file/data
	eTagAnimation = 'ANIM', // tag for animation file/data
	eTagString = 'STR ', // tag for string data
	eTagGroup = 'GRUP', // tag for grouped data
	eTagWorld = 'WRLD', // tag for world data
	eTagScript = 'SCRP', // tag for script file/data
	eTagReference = 'REF ', // tag for data reference
	eTagInfo = 'INFO', // tag for data info

};

static_assert(sizeof(uint32_t) == sizeof(char[4]), "uint32_t is not same size as 4 chars");

typedef glm::vec2 Vertex_2f;
typedef glm::vec3 Vertex_3f;


struct Triangle
{
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;
};

struct Header
{
	char tag[4];
	uint32_t version;
	uint32_t size;
};

struct Model_v1
{
	uint32_t vertexCount;
	uint32_t triangleCount;
	
	Vertex_3f* vertices;
	Triangle* triangles;
};

struct Model_v2
{
	uint32_t vertexCount;
	uint32_t triangleCount;

	bool useVertexUV;

	Vertex_3f* vertices;
	Vertex_2f* vertexUV;

	Triangle* triangles;
};

struct Reference_v1
{
	Header referenceHeader; // header for referenced data // presented here to allow quicker searching for specific tags
	uint32_t offset; // offset from start of file
};

#endif