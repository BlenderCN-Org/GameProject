#include "PixelBuffer_gl.hpp"

bool PixelBuffer_gl::init() {
	glCreateBuffers(1, &pixelBuffer);
	return true;
}

void PixelBuffer_gl::release() {
	glDeleteBuffers(1, &pixelBuffer);
	delete this;
}

void PixelBuffer_gl::resize(uint32_t width, uint32_t height) {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, width * height * 4, nullptr, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void PixelBuffer_gl::read(uint32_t width, uint32_t height) {

	savedW = width;
	savedH = height;
	glReadBuffer(GL_FRONT);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void PixelBuffer_gl::getSize(uint32_t &w, uint32_t &h) {
	w = savedW;
	h = savedH;
}

void * PixelBuffer_gl::map() {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	mappedPtr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	return mappedPtr;
}

void PixelBuffer_gl::unMap() {
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	if (mappedPtr) {
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	mappedPtr = nullptr;
}
