#ifndef MEMBUFFER_HPP
#define MEMBUFFER_HPP

#include <cstdint>
#include <memory>

struct MemoryPusher {

public:
	void setBuffer(void* data, uint32_t dataSize);
	void setOffset(uint32_t offset);

	void pushData(void* ptr, uint32_t pushSize);

private:
	uint32_t offset;
	uint32_t totalDataSize;
	void* memoryData;
};

struct MemoryBuffer {

	void deleteBuffer();

	void setData(void* data, uint32_t dataSize);
	void setOffset(uint32_t offset);

	template<typename T>
	T* returnBytes(uint32_t bytesToReturn) {
		if (bytesToReturn > (totalDataSize - offset)) { 
			throw "Trying to read more than avaible in the buffer\n";
		} // should not happen
		if (bytesToReturn == 0) { return nullptr; }	// if you happen to request something of size 0 return nullptr;

		char* data = (char*)memoryData + offset;
		offset += bytesToReturn;

		return (T*)data;
	}

private:

	uint32_t offset;
	uint32_t totalDataSize;
	void* memoryData;

};

#endif