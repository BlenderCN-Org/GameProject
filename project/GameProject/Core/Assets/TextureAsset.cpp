// project includes
#include "TextureAsset.hpp"


void TextureAsset::init() {
	type = AssetType::eAssetType_texture;
	state = AssetState::eAssetState_unloaded;
	dataPtr = nullptr;
	dataSize = 0;
}

void TextureAsset::release() {
	dataSize = 0;
	delete dataPtr;
	dataPtr = nullptr;

	delete this;
}

void TextureAsset::setAssetState(AssetState _state) {
	state = _state;
}

void TextureAsset::setTextureSize(int32_t w, int32_t h) {
	width = w;
	height = h;
}

void TextureAsset::getTextureSize(int32_t & w, int32_t & h) {
	w = width;
	h = height;
}
