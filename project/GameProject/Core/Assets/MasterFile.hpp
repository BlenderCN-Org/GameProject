#ifndef MASTERFILE_HPP
#define MASTERFILE_HPP

#include "Forms\BaseForm.hpp"

#include <cstdint>
#include <fstream>

#include <map>
#include <set>
#include <vector>

struct VersionRecord {
	short major;
	short minor;
};

struct MasterFileHeader {
	char tag[4];
	VersionRecord version;
	uint32_t preloadEntries;
	uint32_t entries;
	uint32_t namedEntries;
};

struct FormOffset {
	uint32_t formID;
	uint32_t offset;
};

struct NamedEntry {
	uint32_t formID;
	uint32_t offset;
	char name[255];
};

struct FormEntry {
	bool deleted;
	BaseForm *form;
};

typedef std::map<uint32_t, uint32_t> FormOffsetTable;
typedef std::map<uint32_t, FormEntry> FormEntries;

class MasterFile
{
	friend class MasterFileExtension;
	friend class DeleteDataExtension;
	friend class EditDataExtension;

public:
	MasterFile();
	~MasterFile();

	bool loadMaster(const char* masterFileName);
	
	/*
	loads data from master file using form id
	*/
	BaseForm* getForm(const uint32_t formID);

	bool isFormPresent(const uint32_t formID);
	bool isFormDeleted(const uint32_t formID);

	uint32_t getFormIDFromNamedSection(const char* name);

	bool isIDTaken(uint32_t formID);

	uint32_t getNextFormID();

	void addForm(uint32_t type, BaseForm* form, uint32_t formID);
	
	uint32_t getNumberUsingType(uint32_t type);
	uint32_t getFormIDFromTypeAndIndex(uint32_t type, uint32_t index);

private:

	const char* fileName;
	std::ifstream masterFileHandle;
	FormOffsetTable offsetTable;
	FormEntries entries;
	std::map<std::string, uint32_t> namedEntriesTable;

	std::map<uint32_t, std::set<uint32_t> > typeMapper;

	uint32_t nextID;

};


#endif