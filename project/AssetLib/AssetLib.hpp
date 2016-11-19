#ifndef ASSETLIB_HPP
#define ASSETLIB_HPP

#include <cstdint>

namespace AssetLib
{
	void* loadWavefrontOBJ(const char* fileName, uint32_t &dataSize);

	void* loadBMP(const char* fileName, int32_t &width, int32_t &height);


}

#endif