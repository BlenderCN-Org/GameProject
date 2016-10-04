#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

#include <Allocators\Allocator.hpp>
#include <new>


class MemoryManager : public Allocator{

public:

	static MemoryManager* getMemoryManager();
	static void release();

	size_t getHeapSize() const;
	size_t getUsedHeap() const;

	bool canAllocate(size_t size) const;

	void* findNextFreeBlock(size_t size);

	virtual void* allocate(size_t size);

#ifdef _DEBUG
	template<typename T>
	T* allocate_d(size_t count, char* file, size_t line) {
		size_t dataSize = (sizeof(T) * count) + sizeof(Block);

		void* ptr = nullptr;

		if (canAllocate(dataSize))
		{
			void* dataPtr = findNextFreeBlock(dataSize);

			ptr = ((char*)dataPtr + sizeof(Block));
			
			Block* b = (Block*)((char*)ptr - sizeof(Block));

			b->file = file;
			b->lineNr = line;
		}

		// use placement new to init object
		return new(ptr) T;
	}
#else

	template<typename T>
	T* allocate(size_t count) {
		size_t dataSize = (sizeof(T) * count) + sizeof(Block);
		
		void* ptr = nullptr;

		if (canAllocate(dataSize))
		{
			void* dataPtr = findNextFreeBlock(dataSize);

			ptr = ((char*)dataPtr + sizeof(Block));
		}

		// use placement new to init object
		return new(ptr) T;
	}

#endif

	void deallocate(void* ptr) {
		if (ptr == nullptr)
			return;

		Block* b = (Block*)((char*)ptr - sizeof(Block));

		usedSize -= b->size;

		Block* p = b->prev;
		Block* n = b->next;
		
		p->next = n;
		if(n)
			n->prev = p;
	}

private:

	struct Block {
		size_t size;
		Block* prev;
		Block* next;
#ifdef _DEBUG
		char* file;
		size_t lineNr;
#endif
	};

	MemoryManager();
	virtual ~MemoryManager();

	static MemoryManager* manager;

	void* heap;
	size_t heapSize;
	size_t usedSize;

};

template<typename T>
T* memAlloc(size_t count) 	{
	return MemoryManager::getMemoryManager()->allocate<T>(count);
}

template<typename T>
T* memAlloc_d(size_t count, char* file, size_t line) {
	return MemoryManager::getMemoryManager()->allocate_d<T>(count, file, line);
}

#ifdef _DEBUG
#define MemoryManager_alloc(T,count) memAlloc_d<T>(count, __FILE__, __LINE__)
#else
#define MemoryManager_alloc(T, count) memAlloc<T>(count);
#endif

#endif