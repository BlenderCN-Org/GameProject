#include "MemoryBuffer.hpp"

namespace Engine {
	namespace Core {

		void MemoryBuffer::deleteBuffer() {
			if (memoryData)
				free(memoryData);
			memoryData = nullptr;
			offset = totalDataSize = 0;
		}

		void MemoryBuffer::setData(void * data, uint32_t dataSize) {
			if (dataSize == 0)
				throw "Data size cannot be 0\n";
			if (data == nullptr)
				throw "Data cannot be null\n";

			memoryData = malloc(dataSize);
			if (memoryData != NULL) {
				memcpy(memoryData, data, dataSize);
			}

			totalDataSize = dataSize;
			offset = 0;
		}

		void MemoryBuffer::setOffset(uint32_t _offset) {
			if (_offset >= totalDataSize)
				throw "Cannot set offset to end of buffer\n";
			offset = _offset;
		}

		uint32_t MemoryBuffer::getTotalDataSize() {
			return totalDataSize;
		}

		void MemoryPusher::setBuffer(void * data, uint32_t dataSize) {
			if (dataSize == 0)
				throw "Data size cannot be 0\n";
			if (data == nullptr)
				throw "Data cannot be null\n";

			memoryData = data;
			totalDataSize = dataSize;
			offset = 0;
		}

		void MemoryPusher::setOffset(uint32_t _offset) {
			if (_offset >= totalDataSize)
				throw "Cannot set offset to end of buffer\n";
			offset = _offset;
		}

		void MemoryPusher::pushData(void * ptr, uint32_t pushSize) {
			char* data = (char*)memoryData;

			if (pushSize > (totalDataSize - offset)) { throw "Trying to push more data than buffer allows\n"; } // should not happen
			if (pushSize == 0) { return; } // if size is 0 (if string size is 0 for example)
			if (ptr == nullptr) { return; } // if data is null for some reason (could happen with strings for example)

			memcpy(&data[offset], ptr, pushSize);

			offset += pushSize;
		}

		uint32_t MemoryPusher::getOffset() const {
			return offset;
		}

	}
}