/// Internal Includes
#include "MapLoader.hpp"

/// External Includes

/// Std Includes

/***************************************
 *			Template helper			   *
 ***************************************/

template<typename T>
std::istream& readHelper(std::ifstream& file, T* store, size_t size) {
	return file.read((char*)store, size);
}

/***************************************
*			Class Functions			   *
***************************************/

namespace Engine {
	namespace DataLoader {

		MapLoader::MapLoader(Engine::AssetHandling::IAssetManager& assetManager) : assetMan(assetManager) {}

		MapLoader::~MapLoader() {

		}

		void MapLoader::openFile(const char* file) {
			dataFile.open(file);

			loadOffsets();
		}

		void MapLoader::closeFile() {
			dataFile.close();
		}

		Engine::AssetHandling::EntryData MapLoader::loadEntry(uint32_t entryId) {
			Engine::AssetHandling::EntryData ret;

			ret.entryId = 0;

			ret.data = nullptr;
			ret.size = 0U;
			ret.tag = Engine::AssetHandling::NULL_TAG;

			if (assetMan.isEntryLoaded(entryId)) {
				ret = assetMan.loadEntry(entryId);
			} else if (dataFile.is_open()) {
				// entry must be in the offset map
				if (offsetMap.count(entryId)) {

					dataFile.seekg(offsetMap[entryId]);
					uint32_t id;
					uint32_t size;
					Engine::AssetHandling::Tag t;

					readHelper(dataFile, &id, sizeof(uint32_t));
					readHelper(dataFile, &t, sizeof(Engine::AssetHandling::Tag));
					readHelper(dataFile, &size, sizeof(uint32_t));

					if (size > 0U) {

						ret.entryId = id;

						ret.size = size;
						ret.tag = t;
						ret.data = malloc(size);
						ret.engineFlagsLow = 0;
						ret.engineFlagsHigh = 0;

						readHelper(dataFile, ret.data, size);
						assetMan.cacheData(entryId, ret);
					}
				}
			}

			return ret;
		}

		void MapLoader::freeEntry(Engine::AssetHandling::EntryData& data) {
			assetMan.unloadEntry(data);
		}

		/************************************************
		 *            Private Functions					*
		 ************************************************/

		void MapLoader::loadOffsets() {

			if (dataFile.is_open()) {

				Header h;
				readHelper(dataFile, &h, sizeof(Header));

				numEntries = h.numEntries;

				OffsetData offData;

				for (size_t i = 0; i < numEntries; i++) {

					readHelper(dataFile, &offData, sizeof(OffsetData));

					offsetMap[offData.entry] = offData.offset;
				}
			}
		}

	} // DataLoader
} // Engine