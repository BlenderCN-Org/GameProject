#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

/// Internal Includes
#include "Graphics/Textures/Texture2D.hpp"
#include "Interfaces/IAssetManager.hpp"
#include "Utils/MemoryBuffer.hpp"

/// External Includes

/// Std Includes
#include <map>

namespace Engine {
	class AssetManager : public Interfaces::IAssetManager {

	public:

		virtual ~AssetManager();

		virtual Interfaces::ICMesh* loadMesh(const char* model);
		virtual Graphics::Texture::Texture2D* loadTexture(const char* texture);


	private:

		Interfaces::ICMesh* loadMeshV2_0(const char* fileName);

		std::map<std::string, Interfaces::ICMesh*> loadedAssets;
		std::map<std::string, Graphics::Texture::Texture2D*> loadedTextures;
	};
}

#endif