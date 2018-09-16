
/// Internal Includes
#include "EditLoader.hpp"

#include "../../Core/System.hpp"
/// External Includes

/// Std Includes

/***************************************
*			Template helper			   *
***************************************/

template<typename T>
std::istream& readHelper(std::ifstream& file, T* store, size_t size) {
	return file.read((char*)store, size);
}

template<typename T>
std::ostream& writeHelper(std::ofstream& file, T* store, size_t size) {
	return file.write((char*)store, size);
}

namespace Engine {
	namespace DataLoader {

		EditLoader::EditLoader(Engine::AssetHandling::IAssetManager& assetManager) : assetMan(assetManager), numFileEntries(1U) {}

		EditLoader::~EditLoader() {

			uint32_t numLoaded = assetMan.numLoadedEntries();

			if (0 < numLoaded) {
				for (uint32_t i = numLoaded; i > 0; i--) {
					Engine::AssetHandling::EntryData entry = assetMan.entryFromIndex(i - 1);
					assetMan.unloadEntry(entry);
				}
			}

			std::map<uint32_t, AssetHandling::EntryData>::const_iterator it = editedEntries.begin();
			std::map<uint32_t, AssetHandling::EntryData>::const_iterator eit = editedEntries.end();

			for (it; it != eit; ++it) {
				delete it->second.data;
			}

		}

		void EditLoader::save(const char* fileName) {

			backupOld(fileName);

			std::ofstream outFile(fileName, std::ios::binary); // @ TODO: use propper file

			std::vector<AssetHandling::EntryData> entryList = getSortedEntryList();

			Header h;
			h.version = 0x1000;
			h.numEntries = static_cast<uint32_t>(entryList.size());

			writeHelper(outFile, &h, sizeof(Header));

			writeOffsets(outFile, entryList);
			writeEntries(outFile, entryList);

			outFile.close();
		}

		void EditLoader::openFile(const char* fileName) {
			dataFile.open(fileName);

			loadOffsets();

			loadAndRecacheAllEntries();
		}

		void EditLoader::closeFile() {
			dataFile.close();
		}

		bool EditLoader::loading() {
			return false;
		}

		float EditLoader::getLoadingPercentage() {
			return 0.0f;
		}

		uint32_t EditLoader::createNewEntry(AssetHandling::EntryData& data) {
			uint32_t id = findFreeEntryId();

			data.entryId = id;
			data.engineFlagsLow = 0;
			data.engineFlagsHigh = 1;
			editedEntries[id] = data;

			return id;
		}

		void EditLoader::deleteEntry(uint32_t entryId) {

		}

		void EditLoader::editEntry(uint32_t entryId, AssetHandling::EntryData& data) {

			if (editedEntries.count(entryId) != 0) {
				free(editedEntries[entryId].data);
			}

			data.entryId = entryId;
			data.engineFlagsHigh = 1;
			data.engineFlagsLow++;

			editedEntries[entryId] = data;
		}

		Engine::AssetHandling::EntryData EditLoader::loadEntry(uint32_t entryId) {
			Engine::AssetHandling::EntryData ret;

			ret.entryId = 0;

			ret.data = nullptr;
			ret.size = 0U;
			ret.tag = Engine::AssetHandling::NULL_TAG;

			if (editedEntries.count(entryId)) {
				ret = editedEntries[entryId];
			} else if (assetMan.isEntryLoaded(entryId)) {
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

		void EditLoader::freeEntry(Engine::AssetHandling::EntryData& data) {
			//uint64_t combined = System::makeUint64_t(data.engineFlagsHigh, data.engineFlagsLow);
			if (data.engineFlagsHigh == 0) {
				assetMan.unloadEntry(data);
			} else {
				// edit data don't unload original from cache
			}
		}

		/************************************************
		*            Private Functions					*
		************************************************/

		void EditLoader::loadAndRecacheAllEntries() {

			std::map<uint32_t, uint32_t>::const_iterator oit = offsetMap.begin();
			std::map<uint32_t, uint32_t>::const_iterator oeit = offsetMap.end();

			for (oit; oit != oeit; ++oit) {
				//loadEntry(oit->first);

				Engine::AssetHandling::EntryData ret;

				ret.entryId = 0;

				ret.data = nullptr;
				ret.size = 0U;
				ret.tag = Engine::AssetHandling::NULL_TAG;

				uint32_t entryId = oit->first;

				if (dataFile.is_open()) {
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
							
							editEntry(entryId, ret);

						}
					}
				}
			}
		}

		void EditLoader::backupOld(const char* name) {

			std::string fileName = name;

			if (Engine::System::fileExists(fileName.c_str())) {
				std::string bakName = fileName + ".old1";
				if (Engine::System::fileExists(bakName.c_str())) {
					std::string bakName2 = fileName + ".old2";
					if (Engine::System::fileExists(bakName2.c_str())) {
						remove(bakName2.c_str());
					}
					rename(bakName.c_str(), bakName2.c_str());
				}
				rename(fileName.c_str(), bakName.c_str());
			}

		}

		uint32_t EditLoader::findFreeEntryId() {
			return numFileEntries++;
		}

		void EditLoader::loadOffsets() {

			if (dataFile.is_open()) {

				Header h;
				readHelper(dataFile, &h, sizeof(Header));

				numFileEntries = h.numEntries;

				OffsetData offData;

				for (size_t i = 0; i < numFileEntries; i++) {

					readHelper(dataFile, &offData, sizeof(OffsetData));

					offsetMap[offData.entry] = offData.offset;
				}
			}
		}

		std::vector<AssetHandling::EntryData> EditLoader::getSortedEntryList() {

			std::map<uint32_t, AssetHandling::EntryData> newList;

			std::vector<AssetHandling::EntryData> list;

			uint32_t numCachedAssets = assetMan.numLoadedEntries();

			for (uint32_t i = 0; i < numCachedAssets; i++) {
				AssetHandling::EntryData data = assetMan.entryFromIndex(i);
				newList[data.entryId] = data;
			}

			std::map<uint32_t, AssetHandling::EntryData>::const_iterator it = editedEntries.begin();
			std::map<uint32_t, AssetHandling::EntryData>::const_iterator eit = editedEntries.end();

			for (it; it != eit; ++it) {
				newList[it->first] = it->second;
			}

			it = newList.begin();
			eit = newList.end();

			for (it; it != eit; ++it) {
				list.push_back(it->second);
			}

			// @ TODO sort list

			return list;
		}

		void EditLoader::writeOffsets(std::ofstream &outFile, const std::vector<AssetHandling::EntryData> &entries) {

			const uint32_t headerSize = sizeof(Header);
			const uint32_t offsetTableSize = sizeof(OffsetData) * static_cast<uint32_t>(entries.size());

			uint32_t entrySizez = 0;

			for (size_t i = 0; i < static_cast<uint32_t>(entries.size()); i++) {
				AssetHandling::EntryData ed = entries[i];
				OffsetData offData;
				offData.entry = ed.entryId;
				offData.offset = (headerSize + offsetTableSize) + entrySizez;

				writeHelper(outFile, &offData, sizeof(OffsetData));
				entrySizez += (ed.size + sizeof(uint32_t) + sizeof(AssetHandling::Tag) + sizeof(uint32_t));

			}

		}

		void EditLoader::writeEntries(std::ofstream &outFile, const std::vector<AssetHandling::EntryData> &entries) {

			for (size_t i = 0; i < entries.size(); i++) {
				AssetHandling::EntryData ed = entries[i];

				writeHelper(outFile, &ed.entryId, sizeof(uint32_t));
				writeHelper(outFile, &ed.tag, sizeof(AssetHandling::Tag));
				writeHelper(outFile, &ed.size, sizeof(uint32_t));

				writeHelper(outFile, ed.data, ed.size);
			}

		}

	} // DataLoader
} // Engine
