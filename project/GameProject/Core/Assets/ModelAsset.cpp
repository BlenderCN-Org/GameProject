// project includes
#include "ModelAsset.hpp"

// std includes
#include <memory>

void ModelAsset::init() {
	type = AssetType::eAssetType_model;
	state = AssetState::eAssetState_unloaded;
	dataPtr = nullptr;
	dataSize = 0;
}

void ModelAsset::release() {
	dataSize = 0;
	delete dataPtr;
	dataPtr = nullptr;

	delete this;
}

void ModelAsset::setAssetState(AssetState _state) {
	state = _state;
}