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

	void setAssetState(AssetState state);

private:

};

#endif