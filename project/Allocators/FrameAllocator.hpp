#ifndef FRAMEALLOCATOR_HPP
#define FRAMEALLOCATOR_HPP

#include <iostream>
#include "Allocator.hpp"

class FrameAllocator {
private:
	struct HeapStruct {
		void* heap;
		HeapStruct* next;
	};

public:

	FrameAllocator() {
		allocator = nullptr;
		heapSpace = nullptr;
		oldHeap = nullptr;
		maxSize = 0;
		usedSize = 0;

		if (maxSize)
			heapSpace = malloc(maxSize);

		//printf("New framePool %d\n", (int)this);
	}

	FrameAllocator(size_t blockSize, Allocator* _allocator = nullptr) {
		allocator = _allocator;
		heapSpace = nullptr;
		maxSize = blockSize;
		usedSize = 0;
		oldHeap = nullptr;

		if (maxSize) {
			if (allocator)
				heapSpace = allocator->allocate(maxSize);
			else
				heapSpace = malloc(maxSize);
		}
		//printf("New framePool %d\n", (int)this);
	}

	virtual ~FrameAllocator() {
		//printf("Delete framePool %d\n", (int)this);

		if (heapSpace) {
			//printf("Delete pool heap %d (de-tor)\n", (int)heapSpace);
			if (allocator)
				allocator->deallocate(heapSpace);
			else
				free(heapSpace);
		}
		while (oldHeap) {
			//printf("Delete old pool heap %d (de-tor)\n", (int)oldHeap);
			if (oldHeap) {
				if (allocator) {
					allocator->deallocate(oldHeap->heap);
					HeapStruct* hs = oldHeap;
					oldHeap = oldHeap->next;
					allocator->deallocate(hs);
				} else {
					free(oldHeap->heap);
					HeapStruct* hs = oldHeap;
					oldHeap = oldHeap->next;
					free(hs);
				}
			}
		}
		usedSize = 0;
		oldHeap = nullptr;

		heapSpace = nullptr;
	};

	FrameAllocator& operator=(FrameAllocator &newObj) {
		//printf("Copy framePool %d to %d\n", (int)&newObj, (int)this);
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

			HeapStruct* hs = oldHeap;
			oldHeap = newObj.oldHeap;
			newObj.oldHeap = hs;

			Allocator* a = allocator;
			allocator = newObj.allocator;
			newObj.allocator = a;
		}

		return *this;
	}

	template<typename T>
	T* allocate(size_t count) {
		size_t addSize = count * sizeof(T);

		if (usedSize + addSize > maxSize) {
			//printf("\nResizing heap!\n");
			HeapStruct* hs;
			if (allocator)
				hs = (HeapStruct*)allocator->allocate(sizeof(HeapStruct));
			else
				hs = (HeapStruct*)malloc(sizeof(HeapStruct));

			hs->heap = heapSpace;
			hs->next = oldHeap;
			oldHeap = hs;
			maxSize += (1 * KB) + addSize;
			if (allocator)
				heapSpace = allocator->allocate(maxSize);
			else
				heapSpace = malloc(maxSize);

			// if we failed to allocate set our size to 0
			if (!heapSpace)
				maxSize = 0;
		}
		void* address = nullptr;
		if (heapSpace) {
			address = (char*)heapSpace + usedSize;
			usedSize += count * sizeof(T);
		}

		return (T*)address;
	}

	inline void reset() {
		//printf("\r size last frame : %d", usedSize);

		if (oldHeap) {
			if (allocator) {
				allocator->deallocate(oldHeap->heap);
				HeapStruct* hs = oldHeap;
				oldHeap = oldHeap->next;
				allocator->deallocate(hs);
			} else {
				free(oldHeap->heap);
				HeapStruct* hs = oldHeap;
				oldHeap = oldHeap->next;
				free(hs);
			}
			//printf("Deleted outdated buffer\n");
			//oldHeap = nullptr;
		}

		usedSize = 0;
	}

private:

	void* heapSpace;
	HeapStruct* oldHeap;

	size_t maxSize;
	size_t usedSize;

	Allocator* allocator;
};

#endif