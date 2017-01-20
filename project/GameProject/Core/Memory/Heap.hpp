#ifndef HEAP_HPP
#define HEAP_HPP

#define KB 1024
#define MB KB*KB

#include <cstdint>

typedef uint64_t HeapPtr_t;
static_assert(sizeof(HeapPtr_t) == 8, "Ptr is not of correct size");

struct Block {
	uint64_t size;
	HeapPtr_t prev;
	HeapPtr_t next;
};

static_assert(sizeof(Block) == 24, "Block is not of correct size");

struct MemoryDataHeap {
	HeapPtr_t heapPtr;
	uint32_t nrPools;
	HeapPtr_t poolList;
	uint64_t usedSize;
	Block memoryBlock;
};

struct PoolHeap
{
	uint64_t objectTypeID;
	uint32_t nrHeaps;
	HeapPtr_t heapList;
	uint32_t allocateObjects;
	uint32_t maxObjects;
	uint32_t objectsPerHeap;
	uint32_t objectSize;
};

#endif