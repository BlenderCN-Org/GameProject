#include "MemoryBuffer.hpp"

#include <memory>

void MemoryBuffer::deleteBuffer() {
	if ( memoryData )
		delete memoryData;
	memoryData = nullptr;
	offset = totalDataSize = 0;
}

void MemoryBuffer::setData(void * data, uint32_t dataSize) {
	if ( dataSize == 0 )
		throw "Data size cannot be 0\n";
	if( data == nullptr )
		throw "Data cannot be null\n";

	memoryData = malloc(dataSize);
	memcpy(memoryData, data, dataSize);

	totalDataSize = dataSize;
	offset = 0;
}

void MemoryBuffer::setOffset(uint32_t _offset) {
	if ( _offset >= totalDataSize )
		throw "Cannot set offset to end of buffer\n";
	offset = _offset;
}

void * MemoryBuffer::returnBytes(uint32_t bytesToReturn) {
	if ( bytesToReturn > (totalDataSize - offset) ) throw "Trying to read more that than avaible in the buffer\n";
	
	char* data = (char*)memoryData + offset;
	offset += bytesToReturn;

	return data;
}
