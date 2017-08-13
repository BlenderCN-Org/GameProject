#ifndef FILESTRUCTS_HPP
#define FILESTRUCTS_HPP

#include <cstdint>

class IDataObject {
public:

	enum Type {
		GENERIC,
		SCENE,
		RENDERLAYER
	};

	virtual ~IDataObject() {};
	virtual void* getData() const = 0;
	virtual uint32_t getDataSize() const = 0;

	virtual Type getType() const = 0;

};

struct Version {
	uint32_t version;
};

struct Dependency {
	uint8_t dependencyLength;
	char* dependencyName;
};

struct DependencyInfoSave {
	uint8_t nrDependencies;
};

struct DependencyInfo : DependencyInfoSave {
	Dependency* dependencies;
};

struct EntrySave {
	uint32_t formID;
	uint32_t entrySize;
	char tag[4];
};

struct Entry : EntrySave {
	IDataObject* data;		//dataSize = entrySize - sizeof(Entry);
	bool deleteFlag;
};

struct EntryOffset {
	uint32_t formID;
	uint32_t offset;
};

struct GameFileHeader_V1 {
	Version fileVersion;
	uint16_t headerLength;
	uint8_t flags;
	uint8_t padding;
	DependencyInfoSave depInfo;
	uint32_t nrEntries;
};

struct GameFileHeader_V2 {
	Version fileVersion;
	uint16_t headerLength;
	uint8_t flags;
	uint8_t padding;
	DependencyInfoSave depInfo;
	uint32_t nrEntries;
	uint32_t startupSceneRef;
};

/*
* Storable structs
*/

// struct for render layer data
struct RenderLayerSaveData {
	//uint32_t nameLength;
	const char* name;
	char resolutionType;
	uint32_t width;
	uint32_t height;
	bool depthBuffer;
	bool stencilBuffer;
	char nrColorBuffers;
	uint32_t shaderProgramRef;
};

struct SceneSaveData {
	uint32_t sceneNameIdPtr;
};

class IRenderLayerDataObject : public IDataObject {
public:
	virtual ~IRenderLayerDataObject() {};

	virtual RenderLayerSaveData* getRenderLayerData() = 0;
	virtual void setRenderLayerData(RenderLayerSaveData* data) = 0;

	// IDataObject
	virtual void* getData() const = 0;
	virtual uint32_t getDataSize() const = 0;
	
};

class IDataObjectConverter {

public:
	virtual ~IDataObjectConverter() {};

	virtual IRenderLayerDataObject* asRenderLayer(IDataObject*& dataObject) = 0;

};

#endif