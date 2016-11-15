#ifndef BASEASSET_HPP
#define BASEASSET_HPP

// project includes

// std includes
#include <cstdint>

enum class AssetState
{
	eAssetState_unloaded = 0,
	eAssetState_loaded = 1,
	eAssetState_loadedGPU = 2,
};

enum class AssetType
{
	eAssetType_model = 0,
	eAssetType_texture = 1,
	eAssetType_animation = 2,
	eAssetType_other = -1,
};

class BaseAsset
{

public:

	virtual void init() = 0;
	virtual void release() = 0;

	void setDataPtrAndSize(void* dataPtr, uint32_t size);

	uint32_t getDataSize() const;
	void* getDataPtr();

	uint32_t getAssetID() const;
	AssetType getAssetType() const;

	AssetState getAssetState() const;

protected:
	
	uint32_t dataSize;
	void* dataPtr;

	uint32_t assetID;
	AssetType type;
	AssetState state;

};

#endif