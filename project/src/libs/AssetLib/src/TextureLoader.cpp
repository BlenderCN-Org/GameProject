#include <AssetLib/AssetLib.hpp>

#pragma warning( push , 3 )
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#pragma warning( pop )

namespace AssetLib {
	void* loadBMP(const char* fileName, int32_t &width, int32_t &height) {
		int comp = 0;
		return stbi_load(fileName, &width, &height, &comp, 4);
	}

	void* loadTexture(const char* fileName, int32_t &width, int32_t &height, int32_t &comp) {
		void* result = stbi_load(fileName, &width, &height, &comp, 4);
		assert(comp == 4 && "Returned components not equal to requested components!");
		return result;
	}

	void* loadTextureFromMemory(void* data, int32_t dataSize, int32_t &width, int32_t &height, int32_t &comp) {
		void* result = stbi_load_from_memory((stbi_uc*)data, dataSize, &width, &height, &comp, 4);
		assert(comp == 4 && "Returned components not equal to requested components!");
		return result;
	}

	void freeImageData(void* data) {
		stbi_image_free(data);
	}
}