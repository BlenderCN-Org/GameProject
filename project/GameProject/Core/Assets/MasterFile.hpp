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
	
public:
	MasterFile();
	~MasterFile();

	bool loadMaster(const char* masterFileName);

private:

	std::ifstream masterFileHandle;


};


#endif