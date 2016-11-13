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

void ModelAsset::setDataPtrAndSize(void * ptr, uint32_t size) {

	delete dataPtr;

	dataPtr = (void*)new char[size];

	memcpy(dataPtr, ptr, size);

	dataSize = size;
}

uint32_t ModelAsset::getDataSize() const {
	return dataSize;
}

void * ModelAsset::getDataPtr() {
	return dataPtr;
}

uint32_t ModelAsset::getAssetID() const {
	return uint32_t();
}

AssetType ModelAsset::getAssetType() const {
	return type;
}

AssetState ModelAsset::getAssetState() const {
	return state;
}

void ModelAsset::setAssetState(AssetState _state) {
	state = _state;
}
