#include "Mesh_vk.hpp"

void Mesh_vk::init(MeshPrimitiveType p) {}

void Mesh_vk::release() {}

void Mesh_vk::setMeshData(void * data, size_t size, MeshDataLayout layout) {}

void Mesh_vk::bind() {}

void Mesh_vk::render() {}

void * Mesh_vk::map(size_t & dataSize) {
	void * ptr = nullptr;

	//vkMapMemory(device, memory, 0, size, flags, &ptr);

	return ptr;
}

void Mesh_vk::unmap() {
	//vkFlushMappedMemoryRanges(device, rangeCount, memoryRange);
	//vkUnmapMemory(device, memory);
}

void Mesh_vk::flush() {}