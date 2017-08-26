#ifndef HEAP_HPP
#define HEAP_HPP

struct Block {
	size_t size;
	Block* prev;
	Block* next;
};

class Heap {
public:

	Heap();
	virtual ~Heap();

	void tryFree();

	size_t getHeapSize() const;
	size_t getUsedHeap() const;

	bool canAllocate(size_t size) const;
	void* findNextFreeBlock(size_t size);

	void* allocate(size_t size);
	void deallocate(void* ptr);

	void markForDelete();

private:

	bool deleteFlag;
	unsigned int allocatedObjects;

	void* heap;
	size_t heapSize;
	size_t usedSize;
};

#endif