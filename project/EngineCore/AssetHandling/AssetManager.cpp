/// Internal Includes
#include "AssetManager.hpp"

/// External Includes

/// Std Includes

namespace Engine {

	namespace AssetHandling {

		AssetManager::AssetManager(IRenderEngine* renderEngine) {
			pRenderEngine = renderEngine;
			cachedFont = nullptr;
		}

		AssetManager::~AssetManager() {
			cachedFont->release();

			std::map<uint32_t, uint32_t>::const_iterator it = entryRefCounters.begin();
			std::map<uint32_t, uint32_t>::const_iterator eit = entryRefCounters.end();

			uint32_t numFailedCleanups = 0;

			for (it; it != eit; ++it) {
				if (it->second != 0) {
					numFailedCleanups++;
				}
			}

			if (0 != numFailedCleanups) {
				printf("Failed to cleanup and free %lu number of entries\n", numFailedCleanups);
			}

			// @ TODO: free all cached entries and resources
		}

		bool AssetManager::isAssetLoaded(ResourceIdentifier resourceName) const {
			return loadedResources.count(resourceName) != 0;
		}
		AssetData AssetManager::loadAsset(ResourceIdentifier resourceName) {
			return loadedResources[resourceName];
		}
		void AssetManager::unloadAsset(AssetData& assetData) {
			// @ TODO: implement
		}

		uint32_t AssetManager::numLoadedEntries() const {
			return (uint32_t)loadedEntries.size();
		}

		EntryData AssetManager::entryFromIndex(uint32_t idx) const {
			std::map<uint32_t, EntryData>::const_iterator it = loadedEntries.begin();
			std::advance(it, idx);
			return it->second;
		}

		bool AssetManager::isEntryLoaded(uint32_t entryId) const {
			bool loaded = (loadedEntries.count(entryId) != 0);
			return loaded;
		}
		EntryData AssetManager::loadEntry(uint32_t entryId) {
			entryRefCounters[entryId]++;
			return loadedEntries[entryId];
		}
		void AssetManager::unloadEntry(EntryData& entryData) {

			if (entryData.entryId == 0) {
				return;
			}

			entryRefCounters[entryData.entryId]--;
			if (entryRefCounters[entryData.entryId] == 0) {
				loadedEntries.erase(entryData.entryId);
				entryRefCounters.erase(entryData.entryId);
				free(entryData.data);
			}
		}

		void AssetManager::cacheData(uint32_t entryId, EntryData data) {
			entryRefCounters[entryId] = 1;
			loadedEntries[entryId] = data;
		}
		void AssetManager::cacheData(ResourceIdentifier resourceName, AssetData data) {
			loadedResources[resourceName] = data;
		}

		void* AssetManager::loadFont(char* font, int size) {
			if (nullptr == cachedFont) {
				cachedFont = pRenderEngine->createFont();
				cachedFont->init(font, size);
			}
			return cachedFont;
		}

	}
}