#ifndef MEMORYBUFFER_HPP
#define MEMORYBUFFER_HPP

#include <cstdint>

struct MemoryBuffer {

	void deleteBuffer();

	void setData(void* data, uint32_t dataSize);
	void setOffset(uint32_t offset);

	void* returnBytes(uint32_t bytesToReturn);

private:

	uint32_t offset;
	uint32_t totalDataSize;
	void* memoryData;

};

#endif