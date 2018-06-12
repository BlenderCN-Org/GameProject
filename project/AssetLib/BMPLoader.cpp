#include "AssetLib.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

namespace AssetLib {
	void* loadBMP(const char* fileName, int32_t &width, int32_t &height) {
		int comp = 0;

		return stbi_load(fileName, &width, &height, &comp, 4);
	}

	void* loadTexture(const char* fileName, int32_t &width, int32_t &height, int32_t &comp) {
		return stbi_load(fileName, &width, &height, &comp, 4);
	}

	void freeImageData(void* data) {
		stbi_image_free(data);
	}
}