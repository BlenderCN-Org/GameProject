#ifndef ASSETLIB_HPP
#define ASSETLIB_HPP

/// Imternal Includes
#include "ModelStucts.hpp"

/// Std Includes
#include <cstdint>

namespace AssetLib {
	
	enum class ModelVersion {
		VERSION_AUTO,
		VERSION_2_0,
		VERSION_2_1
	};

	const uint32_t SAVE_FLAGS_NO_COMPRESS = 0x0000;
	const uint32_t SAVE_FLAGS_TRY_COMPRESS = 0x0001;
	const uint32_t SAVE_FLAGS_FORCE_COMPRESS = 0x0002;



	struct SaveFlags {
		ModelVersion version;
		uint32_t flags;
	};

	// loads an obj file
	void* loadWavefrontOBJ(const char* fileName, uint32_t &dataSize);

	// loads bmp texture
	void* loadBMP(const char* fileName, int32_t &width, int32_t &height);

	// loads a texture from file
	void* loadTexture(const char* fileName, int32_t &width, int32_t &height, int32_t &comp);

	// loads a texture from memory
	void* loadTextureFromMemory(void* data, int32_t dataSize, int32_t &width, int32_t &height, int32_t &comp);

	void freeImageData(void* data);

	// loads data from any file into memory
	void* fileToMemory(const char* fileName, uint32_t &dataSize);

	Model loadModel(const char* fileName);

	void saveMesh(const char* fileName,const Model& mdl, const SaveFlags& flags);
}

#endif