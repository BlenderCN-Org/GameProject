#ifndef MASTERFILE_HPP
#define MASTERFILE_HPP

#include <cstdint>
#include <fstream>

struct VersionRecord {
	short major;
	short minor;
};

struct MasterFileHeader {
	char tag[4];
	VersionRecord version;
	uint32_t preloadEntries;
	uint32_t entries;
};


class MasterFile
{
	friend class MasterFileExtension;

public:
	MasterFile();
	~MasterFile();

	bool loadMaster(const char* masterFileName);

private:

	const char* fileName;
	std::ifstream masterFileHandle;


};


#endif