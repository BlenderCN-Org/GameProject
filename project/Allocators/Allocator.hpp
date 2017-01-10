#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#define KB 1024
#define MB KB*KB

class Allocator {
public:

	virtual void* allocate(size_t size) = 0;
	virtual void deallocate(void* ptr) = 0;
};

#endif