#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

// project includes
#include "Heap.hpp"

#include "../Exceptions/Exceptions.hpp"

#include <Allocators\FrameAllocator.hpp>

#include <memory>

class MemoryManager
{
public:

	void init();
	void release();

	void saveHeap();
	void loadHeap();

	FrameAllocator* getFrameAllocator();

	void setNumberOfPools(uint32_t poolCount);
	void zeroGameData();

	bool poolExists(int poolObjectTypeID);

private:

	void initHeap();

	HeapPtr_t allocatePhysical(size_t size);
	void deallocatePhysical(HeapPtr_t &ptr);

	bool canAllocate(size_t) const;
	void* findNextFreeBlock(size_t size);

	template<typename T>
	inline T* allocateOnGameHeap(size_t count) {
		size_t dataSize = (sizeof(T) * count) + sizeof(Block);

		void* ptr = nullptr;

		if ( canAllocate(dataSize) ) {
			void* dataPtr = findNextFreeBlock(dataSize);

			ptr = ((char*)dataPtr + sizeof(Block));
			memset(ptr, 0, (sizeof(T) * count));
			memset((char*)ptr + (sizeof(T) * count) - 1, 0xFF, 1);
		}

		// use placement new to init object
		return new(ptr) T;
	}

	inline void deallocateOnGameHeap(void* ptr) {
		if ( ptr == nullptr )
			return;

		Block* b = (Block*)((char*)ptr - sizeof(Block));

		MemoryDataHeap* heapAsPtr = (MemoryDataHeap*)heap;

		heapAsPtr->usedSize -= b->size;

		Block* p = (Block*)b->prev;
		Block* n = (Block*)b->next;

		p->next = (HeapPtr_t)n;
		if ( n )
			n->prev = (HeapPtr_t)p;
	}

	FrameAllocator* fa;

	size_t heapSize;
	HeapPtr_t heap;
};

#endif