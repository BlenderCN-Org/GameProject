#ifndef BASEASSET_HPP
#define BASEASSET_HPP

// project includes

// std includes
#include <cstdint>

enum class AssetState
{
	eAssetState_unloaded = 0,
	eAssetState_loaded = 1,
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

	virtual uint32_t getDataSize() const = 0;
	virtual void* getDataPtr() = 0;

	virtual uint32_t getAssetID() const = 0;
	virtual AssetType getAssetType() const = 0;

	virtual AssetState getAssetState() const = 0;

protected:
	
	uint32_t dataSize;
	void* dataPtr;

	uint32_t assetID;
	AssetType type;
	AssetState state;

};

#endif