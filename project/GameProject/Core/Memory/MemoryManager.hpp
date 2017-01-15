#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

// project includes
#include "Heap.hpp"

#include <Allocators\FrameAllocator.hpp>

class MemoryManager
{
public:

	void init();
	void release();

	void saveHeap();
	void loadHeap();

	FrameAllocator* getFrameAllocator();

private:

	void initHeap();

	HeapPtr_t allocatePhysical(size_t size);
	void deallocatePhysical(HeapPtr_t &ptr);

	FrameAllocator* fa;

	HeapPtr_t heap;
};

#endif