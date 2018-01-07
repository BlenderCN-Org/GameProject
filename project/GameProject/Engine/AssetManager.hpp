#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

/// Internal Includes
#include "Interfaces/IAssetManager.hpp"
#include "Utils/MemoryBuffer.hpp"

/// External Includes

/// Std Includes
#include <map>

namespace Engine {
	class AssetManager : public Interfaces::IAssetManager {

	public:

		virtual Interfaces::ICMesh* loadMesh(const char* model);

	private:

		Interfaces::ICMesh* loadMeshV2_0(const char* fileName);

		std::map<std::string, Interfaces::ICMesh*> loadedAssets;

	};
}

#endif