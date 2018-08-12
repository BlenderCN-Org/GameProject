
#ifndef EDIT_LOADER_HPP
#define EDIT_LOADER_HPP

/// Internal Includes
#include "IEditLoader.hpp"

#include "../IAssetManager.hpp"

/// External Includes

/// Std Includes
#include <fstream>
#include <vector>

namespace Engine {
	namespace DataLoader {

		class EditLoader : public IEditLoader {

		public:

			EditLoader(AssetHandling::IAssetManager& assetManager);
			virtual ~EditLoader();

			// Inherited via IEdit
			virtual void save(const char* fileName) override;

			virtual void openFile(const char* fileName) override;
			virtual void closeFile() override;

			virtual bool loading() override;

			virtual float getLoadingPercentage() override;

			virtual uint32_t createNewEntry(AssetHandling::EntryData& data) override;
			virtual void deleteEntry(uint32_t entryId) override;
			virtual void editEntry(uint32_t entryId, AssetHandling::EntryData& data) override;


			virtual AssetHandling::EntryData loadEntry(uint32_t entryId) override;
			virtual void freeEntry(AssetHandling::EntryData & ld) override;
			
		private:

			void loadAndRecacheAllEntries();

			void backupOld(const char* name);

			uint32_t findFreeEntryId();

			void loadOffsets();

			std::vector<AssetHandling::EntryData> getSortedEntryList();

			void writeOffsets(std::ofstream &outFile, const std::vector<AssetHandling::EntryData> &entries);

			void writeEntries(std::ofstream &outFile, const std::vector<AssetHandling::EntryData> &entries);

			AssetHandling::IAssetManager& assetMan;
			
			// loader
			uint32_t currentFileOffset;

			uint32_t numFileEntries;

			std::map<uint32_t, uint32_t> offsetMap;
			std::ifstream dataFile;

			// editing
			std::map<uint32_t, AssetHandling::EntryData> editedEntries;

		};
	} // DataLoader
} // Engine

#endif