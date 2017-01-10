#include "MemoryManager.hpp"

#include <cstdlib>
#include <iostream>

MemoryManager* MemoryManager::manager = nullptr;

MemoryManager * MemoryManager::getMemoryManager() {
	if ( !manager )
		manager = new MemoryManager();
	return manager;
}

void MemoryManager::release() {
	if ( manager != nullptr ) {
		delete manager;
	}
	manager = nullptr;
}

size_t MemoryManager::getHeapSize() const {
	return heapSize;
}

size_t MemoryManager::getUsedHeap() const {
	return usedSize;
}

bool MemoryManager::canAllocate(size_t size) const {
	bool b = true;

	if ( size + usedSize > heapSize ) {
		b = false;
	}

	return b;
}

void * MemoryManager::findNextFreeBlock(size_t size) {
	void* ptr = heap;

	Block* headerBlock = (Block*)ptr;

	// pointer to the next block
	Block* next = headerBlock->next;

	size_t memLeft = heapSize - sizeof(Block);

	// next contained a pointer we can use that to find free data blocks
	if ( next ) {
		size_t memBetweenBlocks;
		bool loop = true;

		while ( loop ) {
			memBetweenBlocks = ((size_t)next - (size_t)next->prev) - next->prev->size;

			// a free block was found between two blocks and the size was enough to hold data
			if ( memBetweenBlocks >= size ) {
				Block* prev = next->prev;
				Block* me = next;
				Block* b = (Block*)((char*)prev + prev->size);
				usedSize += size;
				b->size = size;

				// prev <-> b <-> me (next)
				prev->next = b;
				me->prev = b;

				b->prev = prev;
				b->next = me;

				return b;
			}
			memLeft -= next->size;

			if ( !next->next )
				loop = false;
			else
				next = next->next;
		}

		// if we don't have memory allocated yet the remaining size is what we have left
		if ( memLeft >= size ) {
			Block* b = (Block*)((char*)next + next->size);
			usedSize += size;
			b->size = size;
			b->next = nullptr;
			b->prev = next;

			next->next = b;
			return b;
		}
		// could not allocate so return nullptr
		return nullptr;
	}
	// next was nullptr so the next free block is right after the header
	else {
		ptr = (char*)ptr + sizeof(Block);
		usedSize += size;
		Block* b = (Block*)ptr;

		b->prev = headerBlock;
		b->next = nullptr;
		b->size = size;
		headerBlock->next = b;
	}

	return ptr;
}

void* MemoryManager::allocate(size_t size) {
	return MemoryManager_alloc(char, size);
}

MemoryManager::MemoryManager() {
	// allocate a memoryblock
	heapSize = 64 * MB;
	heap = malloc(heapSize);
	usedSize = sizeof(Block);

	Block* hd = (Block*)heap;
	// don't have a used block so nullptr markes the state, size used to create new blocks
	hd->size = sizeof(Block);
	hd->next = hd->prev = nullptr;
}

MemoryManager::~MemoryManager() {
	Block* hd = (Block*)heap;

	hd = hd->next;

	//std::cerr << usedSize << "\n";
	bool l = false;
	while ( hd ) {
		std::cerr << "Memory leak detected block: " << std::hex << hd << "\n";
#ifdef _DEBUG
		std::cerr << "File: " << hd->file << "\n";
		std::cerr << "Line: " << std::dec << hd->lineNr << "\n";
#endif
		hd = hd->next;
		l = true;
	}

	if ( l )
		_CrtDbgBreak();
	free(heap);
}