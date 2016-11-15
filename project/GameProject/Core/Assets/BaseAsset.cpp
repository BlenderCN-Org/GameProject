#include "BaseAsset.hpp"

// std includes
#include <memory>

void BaseAsset::setDataPtrAndSize(void * ptr, uint32_t size) {

	delete dataPtr;

	dataPtr = (void*)new char[size];

	memcpy(dataPtr, ptr, size);

	dataSize = size;
}

uint32_t BaseAsset::getDataSize() const {
	return dataSize;
}

void * BaseAsset::getDataPtr() {
	return dataPtr;
}

uint32_t BaseAsset::getAssetID() const {
	return uint32_t();
}

AssetType BaseAsset::getAssetType() const {
	return type;
}

AssetState BaseAsset::getAssetState() const {
	return state;
}