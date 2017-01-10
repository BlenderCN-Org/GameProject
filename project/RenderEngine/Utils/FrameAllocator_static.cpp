#include "FrameAllocator_static.hpp"
#include "MemoryManager.hpp"

FrameAllocator* FrameAllocator_static::singleton;

FrameAllocator * FrameAllocator_static::getFrameAllocator() {
	if ( singleton == nullptr ) {
		singleton = new FrameAllocator(10 * KB, MemoryManager::getMemoryManager());
	}
	return singleton;
}

void FrameAllocator_static::release() {
	if ( singleton ) {
		delete singleton;
	}
	singleton = nullptr;
}