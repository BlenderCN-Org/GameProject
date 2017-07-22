#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "../IAssetManager.hpp"
#include "Assets/GameFile.hpp"

#include <cstdint>

#include <map>

class IMesh;
class IRenderEngine;

class AssetManager : public IAssetManager{

	friend class SaveExtension;

public:

	void init();
	void freeResources();

	uint32_t loadMesh(char* meshName);

	IMesh* getMeshFromInstanceId(uint32_t instanceId);

	uint32_t getNextFormID();

	virtual void createNewEntry(uint32_t formID, const char* tag);
	virtual Entry* getEntry(uint32_t formID);
	virtual void updateEntry(Entry* entry);

	virtual void setStartupSceneRef(uint32_t ref);
	virtual uint32_t getStartupSceneRef();

private:

	std::map<uint32_t, IMesh*> loadedMeshes;
	IRenderEngine* renderEngine;
	uint32_t nrMeshesLoaded;

	GameFile* currentFile;

	uint32_t startupSceneRefId;
};

#endif