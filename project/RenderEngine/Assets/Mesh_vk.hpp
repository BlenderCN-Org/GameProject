#ifndef MESH_VK_HPP
#define MESH_VK_HPP

#include "../IMesh.hpp"
#include <vulkan\vulkan.h>

class Mesh_vk : public IMesh
{
public:

	virtual void init(MeshPrimitiveType p);
	virtual void release();

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout);

	virtual void bind();
	virtual void render();

	virtual void* map(size_t &dataSize);
	virtual void unmap();
	virtual void flush();

private:
	MeshPrimitiveType primitiveType;
	MeshDataLayout dataLayout;

	bool usesIndexBuffer;

	VkBuffer vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory vertexMemory = VK_NULL_HANDLE;

	VkBuffer indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory indexMemory = VK_NULL_HANDLE;

	//GLuint vertexBuffer;
	//GLuint indexBuffer;
	//GLuint vertexArrayObject;
	//
	//GLsizei vertexCount;
	//size_t triangleCount;
	//
	//
	//GLenum meshPrimitive;
	// mapping stuff

	bool isMapped;
	void* mapPtr;
	size_t mappedSize;
};

#endif