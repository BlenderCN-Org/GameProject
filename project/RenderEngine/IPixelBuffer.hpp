#ifndef IPIXELBUFFER_HPP
#define IPIXELBUFFER_HPP

#include <cstdint>

class IPixelBuffer {

public:
	virtual bool init() = 0;
	virtual void release() = 0;

	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual void read(uint32_t width, uint32_t height) = 0;

	virtual void getSize(uint32_t &w, uint32_t &h) = 0;

	virtual void* map() = 0;
	virtual void unMap() = 0;

};

#endif