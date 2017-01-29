#include "AssetLib.hpp"

#include <fstream>

namespace AssetLib {
	void* fileToMemory(const char* fileName, uint32_t &dataSize) {
		dataSize = 0;
		void* data = nullptr;

		std::ifstream inFile;
		inFile.open(fileName, std::ios::binary | std::ios::ate);
		if ( inFile.is_open() ) {
			std::streamoff fileSize = inFile.tellg();
			inFile.seekg(0, inFile.beg);
			data = new char[fileSize];

			memset(data, 0, fileSize);

			inFile.read((char*)data, fileSize);
			dataSize = (uint32_t)fileSize;
		}

		return data;
	}
}