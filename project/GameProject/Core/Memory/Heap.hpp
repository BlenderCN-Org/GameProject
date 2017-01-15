#ifndef HEAP_HPP
#define HEAP_HPP

#define KB 1024
#define MB KB*KB

#include <cstdint>

typedef uint64_t HeapPtr_t;
static_assert(sizeof(HeapPtr_t) == 8, "Ptr is not of correct size");

struct MemoryDataHeap {
	HeapPtr_t heapPtr;
	uint32_t nrPools;
	HeapPtr_t poolList;
};

struct PoolHeap
{
	uint32_t nrHeaps;
	HeapPtr_t heapList;
	uint32_t allocateObjects;
	uint32_t maxObjects;
	uint32_t objectsPerHeap;
	uint32_t objectSize;
};

#endif