#ifndef LOADER_HELPERS_HPP
#define LOADER_HELPERS_HPP

namespace AssetLib {


	void* dataAtOffset(void* ptr, unsigned int &offset, unsigned int readSize, unsigned int dataSize) {

		void* data = nullptr;

		if (offset + readSize <= dataSize) {
			data = (char*)ptr + offset;
			offset += readSize;
		}
		return data;
	}


}

#endif