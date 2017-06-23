#ifndef PIXELBUFFER_GL_HPP
#define PIXELBUFFER_GL_HPP

#include "../IPixelBuffer.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class PixelBuffer_gl : public IPixelBuffer {

public:
	virtual bool init();
	virtual void release();

	virtual void resize(uint32_t width, uint32_t height);
	virtual void read(uint32_t width, uint32_t height);

	virtual void getSize(uint32_t &w, uint32_t &h);

	virtual void* map();
	virtual void unMap();
private:

	GLuint pixelBuffer;
	void* mappedPtr;
	uint32_t savedW;
	uint32_t savedH;

};
#endif