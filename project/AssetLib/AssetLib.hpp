#ifndef ASSETLIB_HPP
#define ASSETLIB_HPP

#include <cstdint>

namespace AssetLib {
	// loads an obj file
	void* loadWavefrontOBJ(const char* fileName, uint32_t &dataSize);

	// loads bmp texture
	void* loadBMP(const char* fileName, int32_t &width, int32_t &height);

	// loads data from any file into memory
	void* fileToMemory(const char* fileName, uint32_t &dataSize);
}

#endif