#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <RenderEngine\IRenderEngine.hpp>

#include <cstdint>
#include <vector>

class AssetManager {

public:

	AssetManager();
	~AssetManager();

	void init(IRenderEngine* renderEngine);
	void release();

	IMesh* getMesh(char* meshName);

	IFont* getBasicFont();

private:

	IRenderEngine* re;
	
	//@Temporary
	// make use of map or heap instead
	std::vector<IMesh*> loadedMeshList;
	
	IFont* basicFont;

};

#endif