#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <RenderEngine\IRenderEngine.hpp>

#include <cstdint>
#include <vector>

#include "MeshAsset.hpp"
#include "MasterFile.hpp"

class AssetManager {
public:

	static AssetManager* getAssetManager();
	static void release();

	AssetManager();
	~AssetManager();

	void init(IRenderEngine* renderEngine);

	IMesh* getMesh(char* meshName);

	IFont* getBasicFont();
	IFont* getSmallFont();

	MasterFile masterFile;

private:

	static AssetManager* assetManager;

	IRenderEngine* re;

	//@Temporary
	// make use of map or heap instead
	std::vector<MeshAsset> loadedMeshList;

	IFont* basicFont;
	IFont* smallFont;

};

#endif