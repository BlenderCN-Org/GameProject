#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

/// Internal Includes

#include "ILoader.hpp"
#include "../../Engine/AssetManager.hpp"
#include "../../Engine/DataTags.hpp"

/// External Includes

/// Std Includes
#include <cstdint>
#include <fstream>
#include <map>

struct LoadedData {
	Engine::AssetManager* assetManPtr;
	uint32_t id;
	uint32_t size;
	Tag tag;
	void* data;
};

struct OffsetData {
	uint32_t entry;
	uint32_t offset;
};


class MapLoader : public ILoader {

public:

	MapLoader(Engine::AssetManager& assetManager);
	virtual ~MapLoader();

	void openMap(const char* file);

	LoadedData loadEntry(uint32_t entryId);
	void freeEntry(LoadedData& ld);

	virtual Engine::Entry loadEntry2(uint32_t entryId);
	virtual void freeEntry(Engine::Entry& ld);

private:

	void loadOffsets();

	uint32_t currentFileOffset;

	uint32_t numEntries;

	std::map<uint32_t, uint32_t> offsetMap;
	std::ifstream mapFile;

	Engine::AssetManager& assetMan;

};

#endif