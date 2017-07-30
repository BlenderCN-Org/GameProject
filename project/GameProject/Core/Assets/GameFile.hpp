#ifndef GAMEFILE_HPP
#define GAMEFILE_HPP

#include "../../FileStructs.hpp"

#include <fstream>
#include <map>
#include <vector>

typedef std::map<uint32_t, uint32_t> OffsetTable;
typedef std::map<uint32_t, Entry> EntryMap;

typedef uint32_t Tag;
typedef std::map<Tag, std::vector<uint32_t>> TagMap;

class GameFile {

	friend class SaveExtension;

public:

	virtual ~GameFile();

	virtual void init();
	virtual void load(const char* name);
	
	Entry* loadEntry(uint32_t formID);

	uint32_t getNextFormID();

	void createNewEntry(uint32_t formID, const char* tag);

	GameFileHeader_V2 getHeaderDataV2() const;

	void loadAllMissingEntries();
	void updateEntry(Entry* e);

	std::vector<uint32_t>& getObjectsWithTag(Tag tag);

protected:

	DependencyInfo dependencies;
	uint32_t nrEntries;
	
	OffsetTable offsetTable;
	EntryMap loadedEntries;

	std::ifstream inFile;

	const char* fileName;

	GameFileHeader_V2 header;

	TagMap tagMap;

};

#endif