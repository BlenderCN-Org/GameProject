#ifndef POOLALLOCATOR_HPP
#define POOLALLOCATOR_HPP

#include <stdint.h>
#include "Allocator.hpp"
#include <iostream>

template <typename T>
class PoolAllocator {
public:

	PoolAllocator() {
		heapSpace = nullptr;
		objectSize = sizeof(T);
		maxSize = 0;
		usedSize = 0;

		//printf("New pool %d\n", (int)this);
	};

	virtual ~PoolAllocator() {
		if (heapSpace) {
			//printf("Delete pool heap %d\n", (int)heapSpace);
			free(heapSpace);
		}
		heapSpace = nullptr;
	};

	PoolAllocator& operator=(PoolAllocator &newObj) {
		//printf("Copy pool %d to %d\n", (int)&newObj, (int)this);
		if (this != &newObj) {
			size_t t = usedSize;
			usedSize = newObj.usedSize;
			newObj.usedSize = t;

			t = maxSize;
			maxSize = newObj.maxSize;
			newObj.maxSize = t;

			void* tp = heapSpace;
			heapSpace = newObj.heapSpace;
			newObj.heapSpace = tp;
		}

		return *this;
	}

	PoolAllocator(int objectCount) {
		//printf("New pool %d\n", (int)this);

		heapSpace = nullptr;
		objectSize = sizeof(T);
		maxSize = objectSize*objectCount;
		maxObjectCount = objectCount;
		usedSize = 0;

		if (maxSize)
			heapSpace = malloc(maxSize);
	}

	T* allocate(int count) {}

	void deallocate(void* ptr, int count) {}

private:

	PoolAllocator<T>* getPtr() {
		return this;
	}

	void setPtr(PoolAllocator* ptr) {
		(PoolAllocator<T>*)this = ptr;
	}

	size_t objectSize;

	void* heapSpace;
	size_t maxSize;
	size_t usedSize;

	size_t objectCount;
	size_t maxObjectCount;
};

#endif