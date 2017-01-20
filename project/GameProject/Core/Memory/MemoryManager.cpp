// project includes
#include "MemoryManager.hpp"

// std includes
#include <malloc.h>
#include <memory>

#include <fstream>

void MemoryManager::init() {

	fa = new FrameAllocator(5 * MB);

	heapSize = 10 * MB;
	heap = allocatePhysical(heapSize);

	memset((void*)heap, 0, sizeof(MemoryDataHeap));

	initHeap();
}

void MemoryManager::release() {
	deallocatePhysical(heap);
	delete fa;
}

void MemoryManager::saveHeap() {
	std::ofstream outFile("Test.mem.sav", std::ios::binary);
	outFile.write((char*)heap, heapSize);
	outFile.close();
}

void MemoryManager::loadHeap() {

	// copy old heap
	HeapPtr_t oldHeap = heap;
	HeapPtr_t newHeap = (HeapPtr_t)nullptr;

	std::ifstream inFile("Test.mem.sav", std::ios::binary | std::ios::ate);

	std::streamoff size = inFile.tellg();

	heapSize = (size_t)size;

	newHeap = allocatePhysical(heapSize);
	inFile.seekg(0, inFile.beg);
	inFile.read((char*)newHeap, heapSize);

	heap = newHeap;
	deallocatePhysical(oldHeap);
	inFile.close();
}

FrameAllocator* MemoryManager::getFrameAllocator() {
	return fa;
}

void MemoryManager::setNumberOfPools(uint32_t poolCount) {
	// sets the number of pools avaible in the game data heap
	MemoryDataHeap* heapAsPtr = (MemoryDataHeap*)heap;
	if ( heapAsPtr->nrPools != 0 ) {
		HeapPtr_t oldPoolHeap = heapAsPtr->poolList;
		heapAsPtr->poolList = (HeapPtr_t)allocateOnGameHeap<PoolHeap>(poolCount);
		PoolHeap* pOldPoolHeap = (PoolHeap*)oldPoolHeap;
		PoolHeap* pNewPoolHeap = (PoolHeap*)heapAsPtr->poolList;

		for ( uint32_t i = 0; i < heapAsPtr->nrPools; i++ ) {
			pNewPoolHeap[i] = pOldPoolHeap[i];
		}

		deallocateOnGameHeap((void*)oldPoolHeap);

		heapAsPtr->nrPools = poolCount;
	} else {
		heapAsPtr->nrPools = poolCount;
		heapAsPtr->poolList = (HeapPtr_t)allocateOnGameHeap<PoolHeap>(poolCount);
	}

}

void MemoryManager::zeroGameData() {
	// delete all stored game data and reinit the heap
	memset((void*)heap, 0, heapSize);
	initHeap();
}

void MemoryManager::initHeap() {
	// heap is a pointer to memory
	// first 8 bytes is the pointer value used to relocate objects on loading

	HeapPtr_t* ptrValue = &heap;
	MemoryDataHeap* heapAsPtr = (MemoryDataHeap*)heap;

	heapAsPtr->heapPtr = (HeapPtr_t)*ptrValue;
	heapAsPtr->usedSize = sizeof(MemoryDataHeap);
	heapAsPtr->memoryBlock.size = sizeof(Block);
	heapAsPtr->memoryBlock.prev = 0;
	heapAsPtr->memoryBlock.next = 0;

	setNumberOfPools(2);
	setNumberOfPools(3);

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

bool MemoryManager::canAllocate(size_t size) const {
	MemoryDataHeap* heapAsPtr = (MemoryDataHeap*)heap;
	bool b = true;

	if ( size + heapAsPtr->usedSize > heapSize ) {
		b = false;
	}

	return b;
}

void * MemoryManager::findNextFreeBlock(size_t size) {
	void * ptr = nullptr;

	MemoryDataHeap* heapAsPtr = (MemoryDataHeap*)heap;

	Block* headerBlock = &heapAsPtr->memoryBlock;
	// ptr now points on the first MemoryBlock structure
	ptr = (char*)heapAsPtr + sizeof(MemoryDataHeap) - sizeof(Block);

	uint64_t memLeft = heapSize - sizeof(MemoryDataHeap);

	Block* next = (Block*)headerBlock->next;

	if ( next ) {
		uint64_t memBetweenBlocks = 0;
		bool loop = true;

		while ( loop ) {
			memBetweenBlocks = ((uint64_t)next - (uint64_t)next->prev) - ((Block*)next->prev)->size;

			// a free block was found between two blocks and the size was enough to hold data
			if ( memBetweenBlocks >= size ) {
				Block* prev = (Block*)next->prev;
				Block* me = next;
				Block* b = (Block*)((char*)prev + prev->size);
				heapAsPtr->usedSize += size;
				b->size = size;

				// prev <-> b <-> me (next)
				prev->next = (HeapPtr_t)b;
				me->prev = (HeapPtr_t)b;

				b->prev = (HeapPtr_t)prev;
				b->next = (HeapPtr_t)me;

				return b;
			}
			memLeft -= next->size;

			if ( !next->next )
				loop = false;
			else
				next = (Block*)next->next;
		}

		// if we don't have memory allocated yet the remaining size is what we have left
		if ( memLeft >= size ) {
			Block* b = (Block*)((char*)next + next->size);
			heapAsPtr->usedSize += size;
			b->size = size;
			b->next = (HeapPtr_t)nullptr;
			b->prev = (HeapPtr_t)next;

			next->next = (HeapPtr_t)b;
			return b;
		}
		// could not allocate so return nullptr
		return nullptr;

	}
	// next was nullptr so the next free block is right after the header
	else {
		ptr = (char*)ptr + sizeof(Block);
		//usedSize += size;
		Block* b = (Block*)ptr;
		memset(b, 0xFF, sizeof(Block));

		b->prev = (HeapPtr_t)headerBlock;
		b->next = (HeapPtr_t)nullptr;
		b->size = size;
		headerBlock->next = (HeapPtr_t)b;
	}

	return ptr;
}
