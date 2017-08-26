#ifndef DATAOBJECTS_HPP
#define DATAOBJECTS_HPP

#include "../../FileStructs.hpp"

class DataObjectConverter : public IDataObjectConverter {
public:
	virtual ~DataObjectConverter() {};

	virtual IRenderLayerDataObject* asRenderLayer(IGameDataObject*& dataObject);
	virtual ISceneDataObject* asSceneData(IGameDataObject*& dataObject);
	virtual IStaticObjectDataObject* asStaticObjectData(IGameDataObject*& dataObject);
};

class DataObject : public IGameDataObject {
public:
	DataObject(void* data, uint32_t dataSize);
	virtual ~DataObject();

	void* getData() const;
	uint32_t getDataSize() const;

	virtual Type getType() const;

private:
	uint32_t mDataSize;
	void* mData;
};

class RenderLayerDataObject : public IRenderLayerDataObject {
public:

	RenderLayerDataObject(void* data, uint32_t dataSize);
	virtual ~RenderLayerDataObject();

	virtual RenderLayerSaveData* getRenderLayerData();
	virtual void setRenderLayerData(RenderLayerSaveData* data);

	// IGameDataObject
	virtual void* getData() const;
	virtual uint32_t getDataSize() const;

	Type getType() const;

private:

	RenderLayerSaveData* rData;
	void* rawData;
	uint32_t rawDataSize;
};

class SceneDataObject : public ISceneDataObject {
public:

	SceneDataObject(void* data, uint32_t dataSize);
	virtual ~SceneDataObject();

	virtual SceneSaveData* getSceneData();
	virtual void setSceneData(SceneSaveData* data);

	// IGameDataObject
	virtual void* getData() const;
	virtual uint32_t getDataSize() const;

	Type getType() const;

private:

	SceneSaveData* rData;
	void* rawData;
	uint32_t rawDataSize;
};

class StaticObjectDataObject : public IStaticObjectDataObject {
public:

	StaticObjectDataObject(void* data, uint32_t dataSize);
	virtual ~StaticObjectDataObject();

	virtual StaticObjectSaveData* getStaticObjectData();
	virtual void setStaticObjectData(StaticObjectSaveData* data);

	// IGameDataObject
	virtual void* getData() const;
	virtual uint32_t getDataSize() const;

	Type getType() const;

private:

	StaticObjectSaveData* rData;
	void* rawData;
	uint32_t rawDataSize;
};

#endif