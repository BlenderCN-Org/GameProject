#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

// project includes
#include "BaseAsset.hpp"
#include "../ThreadManager.hpp"

// std includes
#include <vector>
#include <fstream>

class AssetManager
{

public:

	void setThreadManager(ThreadManager* threadManager);

private:
	std::vector<BaseAsset*> assetList;

	ThreadManager* threadManager;
};

#endif