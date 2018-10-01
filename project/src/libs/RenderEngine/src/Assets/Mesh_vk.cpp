#include "Mesh_vk.hpp"

Mesh_vk::Mesh_vk()
#if SUPPORT_VULKAN_WINDOW
	: vertexBuffer(VK_NULL_HANDLE)
	, vertexMemory(VK_NULL_HANDLE)
	, indexBuffer(VK_NULL_HANDLE)
	, indexMemory(VK_NULL_HANDLE)
#endif
{}

void Mesh_vk::init(MeshPrimitiveType ) {}

void Mesh_vk::release() {}

void Mesh_vk::setMeshData(void * , size_t , MeshDataLayout ) {}

void Mesh_vk::setMeshData(void* , size_t , DataLayout* , unsigned char ) {}

float Mesh_vk::getRadius() {
	return 0;
}

void Mesh_vk::bind() {}

void Mesh_vk::render() {}

void * Mesh_vk::map(size_t & ) {
	void * ptr = nullptr;

	//vkMapMemory(device, memory, 0, size, flags, &ptr);

	return ptr;
}

void Mesh_vk::unmap() {
	//vkFlushMappedMemoryRanges(device, rangeCount, memoryRange);
	//vkUnmapMemory(device, memory);
}

void Mesh_vk::flush() {}