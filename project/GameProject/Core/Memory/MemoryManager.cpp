// project includes
#include "MemoryManager.hpp"

// std includes
#include <malloc.h>
#include <memory>

#include <fstream>

void MemoryManager::init() {

	fa = new FrameAllocator(5 * MB);

	heap = allocatePhysical(MB);

	memset((void*)heap, 0, MB);

	initHeap();
}

void MemoryManager::release() {
	deallocatePhysical(heap);
	delete fa;
}

void MemoryManager::saveHeap() {
	std::ofstream outFile("Test.mem.sav", std::ios::binary);
	outFile.write((char*)heap, MB);
	outFile.close();
}

void MemoryManager::loadHeap() {

	// copy old heap
	HeapPtr_t oldHeap = heap;
	HeapPtr_t newHeap = (HeapPtr_t)nullptr;

	std::ifstream inFile("Test.mem.sav", std::ios::binary | std::ios::ate);

	std::streamoff size = inFile.tellg();

	newHeap = allocatePhysical(size);
	inFile.seekg(0, inFile.beg);
	inFile.read((char*)newHeap, size);

	heap = newHeap;
	deallocatePhysical(oldHeap);
	inFile.close();
}

FrameAllocator* MemoryManager::getFrameAllocator() {
	return fa;
}

void MemoryManager::initHeap() {
	// heap is a pointer to memory
	// first 8 bytes is the pointer value used to relocate objects on loading

	HeapPtr_t* ptrValue = &heap;
	HeapPtr_t* heapAsPtr = (HeapPtr_t*)heap;

	heapAsPtr[0] = (HeapPtr_t)*ptrValue;

	int b = 0;

}

HeapPtr_t MemoryManager::allocatePhysical(size_t size) {
	void* p = malloc(size);
	return (HeapPtr_t)p;
}

void MemoryManager::deallocatePhysical(HeapPtr_t & ptr) {
	free((void*)ptr);
	ptr = (HeapPtr_t)nullptr;
}
