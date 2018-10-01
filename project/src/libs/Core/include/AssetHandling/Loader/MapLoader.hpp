#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

/// Internal Includes

#include "ILoader.hpp"
#include "../AssetManager.hpp"

/// External Includes

/// Std Includes
#include <cstdint>
#include <fstream>
#include <map>

namespace Engine {
	namespace DataLoader {

		struct LoadedData {
			Engine::AssetHandling::IAssetManager* assetManPtr;
			uint32_t id;
			uint32_t size;
			Engine::AssetHandling::Tag tag;
			void* data;
		};

		class MapLoader : public ILoader {

		public:

			MapLoader(Engine::AssetHandling::IAssetManager& assetManager);
			virtual ~MapLoader();

			virtual void openFile(const char* fileName) override;
			virtual void closeFile() override;

			virtual Engine::AssetHandling::EntryData loadEntry(uint32_t entryId) override;
			virtual void freeEntry(Engine::AssetHandling::EntryData& ld) override;

		private:

			void loadOffsets();

			uint32_t currentFileOffset;

			uint32_t numEntries;

			std::map<uint32_t, uint32_t> offsetMap;
			std::ifstream dataFile;

			Engine::AssetHandling::IAssetManager& assetMan;

		};
	} // DataLoader
} // Engine

#endif