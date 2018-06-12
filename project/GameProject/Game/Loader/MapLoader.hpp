#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

/// Internal Includes

/// External Includes

/// Std Includes
#include <cstdint>
#include <map>
#include <fstream>

struct Tag {
	char tag[4];

	bool operator==(const Tag& right) {
		if (tag[0] == right.tag[0] &&
			tag[1] == right.tag[1] &&
			tag[2] == right.tag[2] &&
			tag[3] == right.tag[3]) {
			return true;
		}
		return false;
	}
};

struct LoadedData {
	uint32_t size;
	Tag tag;
	void* data;
};

struct OffsetData {
	uint32_t entry;
	uint32_t offset;
};

/// Constants
const Tag NULL_TAG { 'N', 'U', 'L', 'L' };
const Tag MAP_TAG { 'M', 'A', 'P', ' ' };
const Tag SKY_TAG { 'S', 'K', 'Y', ' ' };
const Tag CELL_TAG { 'C', 'E', 'L', 'L' };
const Tag OBJ_TAG { 'O', 'B', 'J', ' ' };
const Tag MESH_TAG { 'M', 'E', 'S', 'H' };

class MapLoader {

public:

	MapLoader();
	virtual ~MapLoader();

	void openMap(const char* file);

	LoadedData loadDataEntry(uint32_t entryId);

	void freeData(LoadedData& ld);

private:

	void loadOffsets();

	uint32_t currentFileOffset;

	uint32_t numEntries;

	std::map<uint32_t, uint32_t> offsetMap;
	std::ifstream mapFile;


};

#endif