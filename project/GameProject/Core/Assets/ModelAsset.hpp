#ifndef MODELASSET_HPP
#define MODELASSET_HPP

// project includes
#include "BaseAsset.hpp"

// std includes

class ModelAsset : public BaseAsset
{

public:

	virtual void init();
	virtual void release();

	void setDataPtrAndSize(void* dataPtr, uint32_t size);

	virtual uint32_t getDataSize() const;
	virtual void* getDataPtr();

	virtual uint32_t getAssetID() const;
	virtual AssetType getAssetType() const;

	virtual AssetState getAssetState() const;

	void setAssetState(AssetState state);

private:

};

#endif