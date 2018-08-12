#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

/// Internal Includes
#include "IAssetManager.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <map>

namespace Engine {
	
	namespace AssetHandling {
				
		class AssetManager : public IAssetManager {

		public:

			AssetManager(IRenderEngine* renderEngine);

			virtual ~AssetManager();

			virtual bool isAssetLoaded(ResourceIdentifier resourceName) const override;
			virtual AssetData loadAsset(ResourceIdentifier resourceName) override;
			virtual void unloadAsset(AssetData& assetData) override;

			virtual uint32_t numLoadedEntries() const override;
			virtual EntryData entryFromIndex(uint32_t idx) const override;

			virtual bool isEntryLoaded(uint32_t entryId) const override;
			virtual EntryData loadEntry(uint32_t entryId) override;
			virtual void unloadEntry(EntryData& entryData) override;

			virtual void cacheData(uint32_t entryId, EntryData data) override;
			virtual void cacheData(ResourceIdentifier resourceName, AssetData data) override;

			virtual void* loadFont(char* font, int size) override;

		private:

			std::map<uint32_t, EntryData> loadedEntries;
			std::map<uint32_t, uint32_t> entryRefCounters;
			std::map<ResourceIdentifier, AssetData> loadedResources;

			IReFont* cachedFont;
			IRenderEngine* pRenderEngine;

		};
	}
}

#endif