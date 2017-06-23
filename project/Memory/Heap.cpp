#include "Heap.hpp"
#include <memory>
#include <iostream>

#define KB 1024
#define MB KB * KB

Heap::Heap() {
	// allocate a memoryblock
	heapSize = 64 * MB;
	heap = malloc(heapSize);
	usedSize = sizeof(Block);

	Block* hd = (Block*)heap;
	// don't have a used block so nullptr marks the state, size used to create new blocks
	hd->size = sizeof(Block);
	hd->next = hd->prev = nullptr;
	deleteFlag = false;
	allocatedObjects = 0;
}

Heap::~Heap() {

	Block* hd = (Block*)heap;

	hd = hd->next;

	//std::cerr << usedSize << "\n";
	bool leak = false;
	while (hd) {
		std::cerr << "Memory leak detected block: " << std::hex << hd << "\n";
		//#ifdef _DEBUG
		//		std::cerr << "File: " << hd->file << "\n";
		//		std::cerr << "Line: " << std::dec << hd->lineNr << "\n";
		//#endif
		hd = hd->next;
		leak = true;
	}

	//if (leak)
	//	_CrtDbgBreak();
	free(heap);

}

void Heap::tryFree() {
	if (deleteFlag) {
		if (allocatedObjects == 0) {
			delete this;
		}
	}
}

size_t Heap::getHeapSize() const {
	return heapSize;
}

size_t Heap::getUsedHeap() const {
	return usedSize;
}

bool Heap::canAllocate(size_t size) const {
	bool canAlloc = true;

	if (size + usedSize > heapSize) {
		canAlloc = true;
	}

	return canAlloc;
}

void * Heap::findNextFreeBlock(size_t size) {

	void* ptr = heap;

	Block* headerBlock = (Block*)ptr;

	// pointer to the next block
	Block* next = headerBlock->next;

	size_t memLeft = heapSize - sizeof(Block);

	// next contained a pointer we can use that to find free data blocks
	if (next) {
		size_t memBetweenBlocks;
		bool loop = true;

		while (loop) {
			memBetweenBlocks = ((size_t)next - (size_t)next->prev) - next->prev->size;

			// a free block was found between two blocks and the size was enough to hold data
			if (memBetweenBlocks >= size) {
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

			if (!next->next)
				loop = false;
			else
				next = next->next;
		}

		// if we don't have memory allocated yet the remaining size is what we have left
		if (memLeft >= size) {
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

void* Heap::allocate(size_t size) {

	size_t allocSize = size + sizeof(Block);

	void* ptr = nullptr;
	if (canAllocate(allocSize)) {
		ptr = findNextFreeBlock(allocSize);
		if (ptr) {
			allocatedObjects++;
			// offset ptr
			ptr = ((char*)ptr + sizeof(Block));
			memset(ptr, 0xDE, size);
		}
	}

	return ptr;
}

void Heap::deallocate(void* ptr) {
	if (ptr == nullptr)
		return;

	if (heap > ptr || ptr > ((char*)heap + heapSize)) {
		std::cerr << "Trying to delete pointer outside of heap space\n";
		std::cerr << "Invalid ptr: 0x" << std::hex << ptr << "\n";
		std::cerr << "Heap start: 0x" << std::hex << heap << "\n";
		std::cerr << "Heap end: 0x" << std::hex << (void*)((char*)heap + heapSize) << "\n";
		throw;
	}

	// rewind to block ptr
	Block* b = (Block*)((char*)ptr - sizeof(Block));

	usedSize -= b->size;
	allocatedObjects--;
	Block* p = b->prev;
	Block* n = b->next;

	p->next = n;
	if (n) {
		n->prev = p;
	}

	tryFree();

}

void Heap::markForDelete() {
	deleteFlag = true;
}
