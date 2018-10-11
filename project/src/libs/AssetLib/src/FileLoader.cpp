#include <AssetLib/AssetLib.hpp>

#include <fstream>
#include <cstring>

namespace AssetLib {
	void* fileToMemory(const char* fileName, uint32_t &dataSize) {
		dataSize = 0;
		void* data = nullptr;

		std::ifstream inFile;
		inFile.open(fileName, std::ios::binary | std::ios::ate);
		if (inFile.is_open()) {
			std::streamoff fileSize = inFile.tellg();
			inFile.seekg(0, inFile.beg);
			data = new char[(size_t)fileSize];

			memset(data, 0, (size_t)fileSize);

			inFile.read((char*)data, fileSize);
			dataSize = (uint32_t)fileSize;
		}

		return data;
	}
}