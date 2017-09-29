#include "Buffer_gl.hpp"
#include "../ReGlobal.hpp"


void Buffer_gl::init() {

}
void Buffer_gl::free() {

}

bool Buffer_gl::lock() {
	return thrdLock.lock();
}
void Buffer_gl::unlock() {
	thrdLock.unlock();
}

uint32_t Buffer_gl::getDataSize() const {

	uint32_t dataSize = 0U;

	if (thrdLock.hasLock()) {

	}

	return dataSize;
}

void*Buffer_gl::getDataPtr() const {

	void* dataPtr = nullptr;

	if (thrdLock.hasLock()) {

	}

	return dataPtr;
}

void Buffer_gl::setData(uint32_t dataSize, void* data) {

	if (thrdLock.hasLock()) {

		auto fn = []() {};

		// if render thread do direct access
		if (activeThread == getThreadId()) {
			fn();
		}
		// else we need to notify render thread to do the work
		else {
			postGlFunction(fn);
		}
	}

}