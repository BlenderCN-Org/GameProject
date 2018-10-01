#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

/// Internal Includes
#include "DataTags.hpp"

#include "Graphics/Textures/Texture2D.hpp"
#include "Interfaces/IAssetManager.hpp"
#include "Utils/MemoryBuffer.hpp"

/// External Includes

/// Std Includes
#include <map>

namespace Engine {
	
	struct Entry {
		uint32_t id;
		uint32_t refCounter;
		Tag tag;
		uint32_t size;
		void* data;
	};

	class AssetManager : public Interfaces::IAssetManager {

	public:

		virtual ~AssetManager();

		virtual Interfaces::ICMesh* loadMesh(const char* model);
		virtual Graphics::Texture::Texture2D* loadTexture(const char* texture);

		virtual bool isLoaded(uint32_t entryId);
		virtual void cacheEntry(uint32_t entryId, Entry dataPtr);
		virtual Entry getCachedEntry(uint32_t entryId);

		//virtual void incReferenceCount(uint32_t entryId);
		virtual void decRerferenceCount(uint32_t entryId);

	private:

		Interfaces::ICMesh* loadMeshV2_0(const char* fileName);

		std::map<std::string, Interfaces::ICMesh*> loadedAssets;
		std::map<std::string, Graphics::Texture::Texture2D*> loadedTextures;

		std::map<uint32_t, Entry> loadedEntries;

	};
}

#endif