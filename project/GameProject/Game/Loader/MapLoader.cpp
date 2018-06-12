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

MapLoader::MapLoader() {

}

MapLoader::~MapLoader() {

}

void MapLoader::openMap(const char* file) {
	mapFile.open(file);

	loadOffsets();

}

LoadedData MapLoader::loadDataEntry(uint32_t entryId) {

	LoadedData ret;

	ret.data = nullptr;
	ret.size = 0U;
	ret.tag = NULL_TAG;

	if (mapFile.is_open()) {

		// entry must be in the offset map
		if (offsetMap.count(entryId)) {

			mapFile.seekg(offsetMap[entryId]);
			uint32_t id;
			uint32_t size;
			Tag t;

			readHelper(mapFile, &id, sizeof(uint32_t));
			readHelper(mapFile, &t, sizeof(Tag));
			readHelper(mapFile, &size, sizeof(uint32_t));

			if (size > 0U) {

				ret.size = size;
				ret.tag = t;
				ret.data = malloc(size);

				readHelper(mapFile, ret.data, size);
			}


		}
	}

	return ret;
}

void MapLoader::freeData(LoadedData& ld) {
	free(ld.data);
}

/************************************************
 *            Private Functions					*
 ************************************************/

void MapLoader::loadOffsets() {

	if (mapFile.is_open()) {

		readHelper(mapFile, &numEntries, sizeof(uint32_t));

		OffsetData offData;

		for (size_t i = 0; i < numEntries; i++) {

			readHelper(mapFile, &offData, sizeof(OffsetData));

			offsetMap[offData.entry] = offData.offset;
		}

	}

}