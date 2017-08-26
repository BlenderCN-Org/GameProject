#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "../IAssetManager.hpp"
#include "Assets/GameFile.hpp"

#include <cstdint>

#include <map>

class IMesh;
class IRenderEngine;

class AssetManager : public IAssetManager {
	friend class SaveExtension;

public:

	void init();
	void freeResources();

	uint32_t loadMesh(char* meshName);

	IMesh* getMeshFromInstanceId(uint32_t instanceId);

	virtual uint32_t getNextFormID();

	virtual uint32_t getNumEntries();
	virtual EntryMap &getAllEntries();

	virtual void createNewEntry(uint32_t formID, const char* tag);
	virtual void createNewEntry(uint32_t formID, const char* tag, void* data, uint32_t dataSize);
	virtual Entry* getEntry(uint32_t formID);
	virtual void updateEntry(Entry* entry);
	virtual void updateEntry(uint32_t formID, void* data, uint32_t dataSize);

	virtual void setStartupSceneRef(uint32_t ref);
	virtual uint32_t getStartupSceneRef();

	virtual std::vector<uint32_t>& getObjectsWithTag(Tag tag);
	virtual uint32_t getObjectCountWithTag(Tag tag);

	virtual IDataObjectConverter* getConverter() const;

private:

	std::map<uint32_t, IMesh*> loadedMeshes;
	IRenderEngine* renderEngine;
	uint32_t nrMeshesLoaded;

	GameFile* currentFile;

	uint32_t startupSceneRefId;

	DataObjectConverter objectConverter;
};

#endif