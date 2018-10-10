#ifndef IASSETMANAGER_HPP
#define IASSETMANAGER_HPP

#include "FileStructs.hpp"

#include <vector>
#include <map>

static const char* RENDERLAYER_TAG = "REND";
static const char* SCENE_TAG = "SCNE";
static const char* STRING_TAG = "STR ";
static const char* STATICOBJECT_TAG = "SOBJ";

typedef uint32_t Tag;
typedef std::map<uint32_t, Entry> EntryMap;

class IAssetManager {
public:

	virtual uint32_t getNextFormID() = 0;

	virtual uint32_t getNumEntries() = 0;
	virtual EntryMap &getAllEntries() = 0;

	virtual void createNewEntry(uint32_t formID, const char* tag) = 0;
	virtual void createNewEntry(uint32_t formID, const char* tag, void* data, uint32_t dataSize) = 0;
	virtual Entry* getEntry(uint32_t formID) = 0;
	virtual void updateEntry(Entry* entry) = 0;
	virtual void updateEntry(uint32_t formID, void* data, uint32_t dataSize) = 0;

	virtual void setStartupSceneRef(uint32_t ref) = 0;
	virtual uint32_t getStartupSceneRef() = 0;

	virtual std::vector<uint32_t>& getObjectsWithTag(Tag tag) = 0;
	virtual uint32_t getObjectCountWithTag(Tag tag) = 0;

	virtual IDataObjectConverter* getConverter() const = 0;
};

#endif