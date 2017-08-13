#include "DataObjects.hpp"
#include "../System/MemBuffer.hpp"
#include <memory>
#include "../System/Sys.hpp"

/*
Data Object Converter
*/

IRenderLayerDataObject* DataObjectConverter::asRenderLayer(IDataObject*& dataObject) {

	IRenderLayerDataObject* obj = nullptr;

	if (dataObject != nullptr) {

		if (dataObject->getType() == IDataObject::RENDERLAYER) { // already of type IRenderLayerDataObject
			obj = (IRenderLayerDataObject*)dataObject;
		} else if (dataObject->getType() == IDataObject::GENERIC) { // Generic can be convertable
			uint32_t size = dataObject->getDataSize();
			obj = new RenderLayerDataObject(dataObject->getData(), size);
			delete dataObject;
			dataObject = obj;
		} else {
			// no convert possible
		}
	} else {
		obj = new RenderLayerDataObject(nullptr, 0);
		dataObject = obj;
	}

	return obj;
}

ISceneDataObject* DataObjectConverter::asSceneData(IDataObject*& dataObject) {

	ISceneDataObject* obj = nullptr;

	if (dataObject != nullptr) {
		if (dataObject->getType() == IDataObject::SCENE) { // already of type IRenderLayerDataObject
			obj = (ISceneDataObject*)dataObject;
		} else if (dataObject->getType() == IDataObject::GENERIC) { // Generic can be convertable
			uint32_t size = dataObject->getDataSize();
			obj = new SceneDataObject(dataObject->getData(), size);
			delete dataObject;
			dataObject = obj;
		} else {
			// no convert possible
		}
	} else {
		obj = new SceneDataObject(nullptr, 0);
		dataObject = obj;
	}
	return obj;
}

/*
Data Object
*/

DataObject::DataObject(void* data, uint32_t dataSize) {
	if (dataSize > 0) {
		mData = malloc(dataSize);
		mDataSize = dataSize;
		memcpy(mData, data, dataSize);
	} else {
		mData = nullptr;
		mDataSize = 0U;
	}
}

DataObject::~DataObject() {
	if (mData) {
		free(mData);
	}
	mData = nullptr;
	mDataSize = 0U;
}

void* DataObject::getData() const {
	return mData;
}

uint32_t DataObject::getDataSize() const {
	return mDataSize;
}

IDataObject::Type DataObject::getType() const {
	return Type::GENERIC;
}

/*
Render Layer Data Object
*/
/*
const char* name;
char resolutionType;
uint32_t width;
uint32_t height;
bool depthBuffer;
bool stencilBuffer;
char nrColorBuffers;
uint32_t shaderProgramRef;
*/

RenderLayerDataObject::RenderLayerDataObject(void * data, uint32_t dataSize) {
	if (dataSize > 0) {
		rawData = malloc(dataSize);
		rawDataSize = dataSize;
		memcpy(rawData, data, dataSize);
	} else {
		rawData = nullptr;
		rawDataSize = 0U;
	}

	const uint32_t saveDataSize = sizeof(RenderLayerSaveData);

	rData = new RenderLayerSaveData();
	memset(rData, 0, saveDataSize);
	if (rawData) {
		// we can asume that the data in buffer is ok for RenderLayerSaveData
		if (rawDataSize >= saveDataSize) {
			MemoryBuffer mBuff;

			mBuff.setData(rawData, rawDataSize);

			uint32_t nameLength = *mBuff.returnBytes<uint32_t>(sizeof(uint32_t));
			rData->name = new char[nameLength];

			// check so that rawData is large enough to also store the name
			if (rawDataSize >= (saveDataSize + nameLength)) {
				memcpy((void*)rData->name, mBuff.returnBytes<char>(nameLength), nameLength);
			}
			// copy rest of data
			rData->resolutionType = *mBuff.returnBytes<char>(sizeof(char));
			rData->width = *mBuff.returnBytes<uint32_t>(sizeof(uint32_t));
			rData->height = *mBuff.returnBytes<uint32_t>(sizeof(uint32_t));
			rData->depthBuffer = *mBuff.returnBytes<bool>(sizeof(bool));
			rData->stencilBuffer = *mBuff.returnBytes<bool>(sizeof(bool));
			rData->nrColorBuffers = *mBuff.returnBytes<char>(sizeof(char));
			rData->shaderProgramRef = *mBuff.returnBytes<uint32_t>(sizeof(uint32_t));

			// delete buffer
			mBuff.deleteBuffer();
		}
	}
}

RenderLayerDataObject::~RenderLayerDataObject() {
	free(rawData);
	delete rData->name;
	delete rData;
}

RenderLayerSaveData * RenderLayerDataObject::getRenderLayerData() {
	return rData;
}

void RenderLayerDataObject::setRenderLayerData(RenderLayerSaveData * data) {

	delete rData->name;
	memcpy(rData, data, sizeof(RenderLayerSaveData));
	rData->name = nullptr;

	uint32_t nameLength = getStringLength(data->name);
	if (nameLength) {
		rData->name = new char[nameLength];
		memcpy((void*)rData->name, (void*)data->name, nameLength);
	}
	// dont store the pointer
	uint32_t newSize = sizeof(RenderLayerSaveData) + nameLength;

	free(rawData);
	rawData = malloc(newSize);
	rawDataSize = newSize;

	MemoryPusher memPush;
	memPush.setBuffer(rawData, rawDataSize);

	memPush.pushData(&nameLength, sizeof(uint32_t));
	memPush.pushData((void*)rData->name, nameLength);
	memPush.pushData(&rData->resolutionType, sizeof(char));
	memPush.pushData(&rData->width, sizeof(uint32_t));
	memPush.pushData(&rData->height, sizeof(uint32_t));
	memPush.pushData(&rData->depthBuffer, sizeof(bool));
	memPush.pushData(&rData->stencilBuffer, sizeof(bool));
	memPush.pushData(&rData->nrColorBuffers, sizeof(char));
	memPush.pushData(&rData->shaderProgramRef, sizeof(uint32_t));

}

void * RenderLayerDataObject::getData() const {
	return rawData;
}

uint32_t RenderLayerDataObject::getDataSize() const {
	return rawDataSize;
}

IDataObject::Type RenderLayerDataObject::getType() const {
	return Type::RENDERLAYER;
}

/*
Scene Data Object
*/
/*
const char* name;
float skyColor[4];
bool hasFog;
float fog[8];
bool hasWater;
*/

SceneDataObject::SceneDataObject(void * data, uint32_t dataSize) {
	if (dataSize > 0) {
		rawData = malloc(dataSize);
		rawDataSize = dataSize;
		memcpy(rawData, data, dataSize);
	} else {
		rawData = nullptr;
		rawDataSize = 0U;
	}

	const uint32_t saveDataSize = sizeof(SceneSaveData);

	rData = new SceneSaveData();
	memset(rData, 0, saveDataSize);
	if (rawData) {
		// we can asume that the data in buffer is ok for RenderLayerSaveData
		if (rawDataSize >= saveDataSize) {
			MemoryBuffer mBuff;

			mBuff.setData(rawData, rawDataSize);

			uint32_t nameLength = *mBuff.returnBytes<uint32_t>(sizeof(uint32_t));
			rData->name = new char[nameLength];

			// check so that rawData is large enough to also store the name
			if (rawDataSize >= (saveDataSize + nameLength)) {
				memcpy((void*)rData->name, mBuff.returnBytes<char>(nameLength), nameLength);
			}
			// copy rest of data
			memcpy(rData->skyColor, mBuff.returnBytes<float>(sizeof(float) * 4), sizeof(float) * 4);
			rData->hasFog = *mBuff.returnBytes<bool>(sizeof(bool));
			memcpy(rData->fog, mBuff.returnBytes<float>(sizeof(float) * 8), sizeof(float) * 8);
			rData->hasWater = *mBuff.returnBytes<bool>(sizeof(bool));

			// delete buffer
			mBuff.deleteBuffer();
		}
	}
}

SceneDataObject::~SceneDataObject() {
	free(rawData);
	delete rData->name;
	delete rData;
}

SceneSaveData * SceneDataObject::getSceneData() {
	return rData;
}

void SceneDataObject::setSceneData(SceneSaveData * data) {

	delete rData->name;
	memcpy(rData, data, sizeof(SceneSaveData));
	rData->name = nullptr;

	uint32_t nameLength = getStringLength(data->name);
	if (nameLength) {
		rData->name = new char[nameLength];
		memcpy((void*)rData->name, (void*)data->name, nameLength);
	}
	// dont store the pointer
	uint32_t newSize = sizeof(SceneSaveData) + nameLength;

	free(rawData);
	rawData = malloc(newSize);
	rawDataSize = newSize;

	MemoryPusher memPush;
	memPush.setBuffer(rawData, rawDataSize);

	memPush.pushData(&nameLength, sizeof(uint32_t));
	memPush.pushData((void*)rData->name, nameLength);
	memPush.pushData(rData->skyColor, sizeof(float) * 4);
	memPush.pushData(&rData->hasFog, sizeof(bool));
	memPush.pushData(rData->fog, sizeof(float) * 8);
	memPush.pushData(&rData->hasWater, sizeof(bool));
}

void * SceneDataObject::getData() const {
	return rawData;
}

uint32_t SceneDataObject::getDataSize() const {
	return rawDataSize;
}

IDataObject::Type SceneDataObject::getType() const {
	return Type::SCENE;
}