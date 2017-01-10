#ifndef TEXTUREASSET_HPP
#define	TEXTUREASSET_HPP

#include "BaseAsset.hpp"

class TextureAsset : public BaseAsset
{
public:

	virtual void init();
	virtual void release();

	void setAssetState(AssetState _state);

	void setTextureSize(int32_t w, int32_t h);

	void getTextureSize(int32_t &w, int32_t &h);

private:
	int32_t width, height;
};

#endif