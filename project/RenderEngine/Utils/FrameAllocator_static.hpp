#ifndef FRAMEALLOCATOR_STATIC_HPP
#define FRAMEALLOCATOR_STATIC_HPP

#include <Allocators\FrameAllocator.hpp>

class FrameAllocator_static : public FrameAllocator{

public:

	static FrameAllocator* getFrameAllocator();
	static void release();

private:

	static FrameAllocator* singleton;

};


#endif