#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

/// Internal Includes
#include "DataTags.hpp"

/// External Includes

/// Std Includes
#include <cstdint>
#include <fstream>
#include <map>

struct LoadedData {
	uint32_t size;
	Tag tag;
	void* data;
};

struct OffsetData {
	uint32_t entry;
	uint32_t offset;
};


class MapLoader {

public:

	MapLoader();
	virtual ~MapLoader();

	void openMap(const char* file);

	LoadedData loadEntry(uint32_t entryId);

	void freeEntry(LoadedData& ld);

private:

	void loadOffsets();

	uint32_t currentFileOffset;

	uint32_t numEntries;

	std::map<uint32_t, uint32_t> offsetMap;
	std::ifstream mapFile;


};

#endif