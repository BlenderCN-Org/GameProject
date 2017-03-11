
#include "AssetLoader.hpp"

#include "../Memory/MemoryBuffer.hpp"

#include <string>
#include <vector>

void* createVertUVData(void* meshData, uint32_t &size) {

	MemoryBuffer memBuff;
	memBuff.setData(meshData, size);

	Header* h = (Header*)memBuff.returnBytes(sizeof(Header));

	int* tag = (int*)h->tag;

	int* meshTag = (int*)"MESH";

	if ( *tag != *meshTag ) {
		printf("Invalid mesh file\n");
		size = 0;
		return nullptr;
	}

	printf("Mesh version: %d.%d\n", h->major, h->minor);

	BoolFlags* bf = (BoolFlags*)memBuff.returnBytes(sizeof(BoolFlags));

	glm::vec3* vertices = (glm::vec3*)memBuff.returnBytes(sizeof(glm::vec3) * bf->vertCount );
	glm::vec3* normals =  bf->useVNormals ? (glm::vec3*)memBuff.returnBytes(sizeof(glm::vec3) * bf->vertCount) : nullptr;
	glm::vec4* colors = bf->useVColors ? (glm::vec4*)memBuff.returnBytes(sizeof(glm::vec4) * bf->vertCount) : nullptr;
	glm::vec2* uv = bf->useVUV ? (glm::vec2*)memBuff.returnBytes(sizeof(glm::vec2) * bf->vertCount) : nullptr;

	Triangle* triangles = (Triangle*)memBuff.returnBytes(sizeof(Triangle) * bf->triangleCount);

	std::vector<Vertex5> verts;

	for ( size_t i = 0; i < bf->triangleCount; i++ ) {
		Triangle t = triangles[i];

		Vertex5 v1 = { vertices[t.v1], glm::vec2(0.0f, 0.0f) };
		Vertex5 v2 = { vertices[t.v2], glm::vec2(0.0f, 0.0f) };
		Vertex5 v3 = { vertices[t.v3], glm::vec2(0.0f, 0.0f) };

		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
	}

	memBuff.deleteBuffer();

	size = (uint32_t)verts.size() * sizeof(Vertex5);

	Vertex5* v = new Vertex5[verts.size()];
	memcpy(v, verts.data(), size);

	return v;
}

