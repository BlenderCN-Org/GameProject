#include "MemoryMan.hpp"
#include "Heap.hpp"
#include <memory>

typedef void* (*alloc)(size_t);
typedef void(*dealloc)(void*);

Heap* h = nullptr;

void* defaultAlloc(size_t size) {
	//printf("Calling malloc!\n");
	return malloc(size);
}

void defaultFree(void* ptr) {
	free(ptr);
}

void* heapAlloc(size_t size) {
	printf("Calling heapAlloc!\n");
	return h->allocate(size);
}

void heapFree(void* ptr) {
	h->deallocate(ptr);
}
void InitMemoryManagement() {
	h = new Heap();
}

void ReleaseMemoryManagement() {

	// heap is allowed to delete itself after all allocations have been released
	h->markForDelete();
	// try to free memory if this fails there are some objects remaining on the heap
	// possible global objects
	h->tryFree();
}

#ifndef DBG_NEW

void* operator new(size_t size) {
	if (size == 0) {
		size = 1;
	}
	void* ans = nullptr;
	//allocatorFn(size + sizeof(void*));	// add size to store a pointer
	if (h) {
		ans = h->allocate(size + sizeof(void*));
		*(void**)ans = h;
	} else {
		ans = malloc(size + sizeof(void*));
		*(void**)ans = nullptr;
	}
	return (char*)ans + sizeof(void*);
}

void* operator new[](size_t size) {
	if (size == 0) {
		size = 1;
	}
	void* ans = nullptr;
	//allocatorFn(size + sizeof(void*));	// add size to store a pointer
	if (h) {
		ans = h->allocate(size + sizeof(void*));
		*(void**)ans = h;
	} else {
		ans = malloc(size + sizeof(void*));
		*(void**)ans = nullptr;
	}
	return (char*)ans + sizeof(void*);
}

void operator delete(void* ptr) {
	if (ptr != nullptr) {
		ptr = (char*)ptr - sizeof(void*);
		if (*(void**)ptr != nullptr) {
			h->deallocate(ptr);
		}
		else {
			free(ptr);
			//printf("Err\n");
		}
	}
}

void operator delete[](void* ptr) {
	if (ptr != nullptr) {
		ptr = (char*)ptr - sizeof(void*);
		if (*(void**)ptr != nullptr) {
			h->deallocate(ptr);
		} else {
			free(ptr);
			//printf("Err\n");
		}
	}
}

#endif