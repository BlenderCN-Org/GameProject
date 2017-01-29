#ifndef ASSETLOADER_HPP
#define ASSETLOADER_HPP

#include <cstdint>
#include <glm\glm.hpp>

// temporary
// move elsewhere

struct Header {
	char tag[4];
	uint16_t major;
	uint16_t minor;
};

struct BoolFlags
{
	uint32_t meshCount;
	bool useVNormals;
	bool useVColors;
	bool useVUV;
	bool padding;
	uint32_t vertCount;
	uint32_t triangleCount;
};

struct Triangle {
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;
};

struct Vertex5
{
	glm::vec3 pos;
	glm::vec2 uv;
};

void* createVertUVData(void* meshData, uint32_t &size);

// end temporary
// end move elsewhere


#endif