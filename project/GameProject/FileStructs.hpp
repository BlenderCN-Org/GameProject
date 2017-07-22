#ifndef FILESTRUCTS_HPP
#define FILESTRUCTS_HPP

#include <cstdint>

struct Version {
	uint32_t version;
};

struct Dependency {
	uint8_t dependencyLength;
	char* dependencyName;
};

struct DependencyInfoSave {
	uint8_t nrDependencies;
};

struct DependencyInfo : DependencyInfoSave {
	Dependency* dependencies;
};

struct EntrySave {
	uint32_t formID;
	uint32_t entrySize;
	char tag[4];
};

struct Entry : EntrySave {
	void* data;		//dataSize = entrySize - sizeof(Entry);
};

struct EntryOffset {
	uint32_t formID;
	uint32_t offset;
};

struct GameFileHeader_V1 {
	Version fileVersion;
	uint16_t headerLength;
	uint8_t flags;
	uint8_t padding;
	DependencyInfoSave depInfo;
	uint32_t nrEntries;
};

struct GameFileHeader_V2 {
	Version fileVersion;
	uint16_t headerLength;
	uint8_t flags;
	uint8_t padding;
	DependencyInfoSave depInfo;
	uint32_t nrEntries;
	uint32_t startupSceneRef;
};

#endif