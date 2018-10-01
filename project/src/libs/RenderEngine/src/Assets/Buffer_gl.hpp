#ifndef BUFFER_GL_HPP
#define BUFFER_GL_HPP

#include "RenderEngine/IBuffer.hpp"
#include "ThreadLock.hpp"

class Buffer_gl : public IBuffer{

public:

	virtual void init();
	virtual void free();

	virtual bool lock() = 0;
	virtual void unlock() = 0;

	virtual uint32_t getDataSize() const = 0;
	virtual void* getDataPtr() const = 0;
	virtual void setData(uint32_t dataSize, void* data) = 0;

private:

	uint32_t glBufferId;

	ThreadLock thrdLock;

};

#endif