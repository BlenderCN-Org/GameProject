#include "../pch.h"

#include <EngineCore/AssetHandling/Loader/EditLoader.hpp>

namespace Asset = Engine::AssetHandling;
namespace Load = Engine::DataLoader;

class TestManager : public Asset::IAssetManager {
public:
	TestManager() {}

	~TestManager() {
		// @ TODO: free all cached entries and resources
	}

	bool isAssetLoaded(Asset::ResourceIdentifier resourceName) const {
		return false;
	}
	Asset::AssetData loadAsset(Asset::ResourceIdentifier resourceName) {
		return Asset::AssetData();
	}
	void unloadAsset(Asset::AssetData& assetData) {
		// @ TODO: implement
	}

	uint32_t numLoadedEntries() const override {
		return 0;
	}
	Asset::EntryData entryFromIndex(uint32_t idx) const override {
		return Asset::EntryData();
	}

	bool isEntryLoaded(uint32_t entryId) const {
		return false;
	}
	Asset::EntryData loadEntry(uint32_t entryId) {
		return Asset::EntryData();
	}
	void unloadEntry(Asset::EntryData& entryData) {
		// @ TODO: implement
	}

	void cacheData(uint32_t entryId, Asset::EntryData data) {}
	void cacheData(Asset::ResourceIdentifier resourceName, Asset::AssetData data) {}
	void* loadFont(char*, int) { return nullptr; }
};

TEST(Engine, LoaderSaveTest) {

	TestManager tm;
	Load::EditLoader ldr(tm);

	char* data = new char[10];
	char* data2 = new char[20];

	Asset::EntryData ed;
	ed.size = sizeof(data);
	ed.data = data;

	memset(data, 'A', sizeof(data));
	ed.tag = { 'M', 'A', 'P', ' ' };
	ldr.createNewEntry(ed);

	ed.size = sizeof(data2);
	ed.data = data2;
	memset(data2, 'B', sizeof(data2));
	ed.tag = { 'S', 'K', 'Y', ' ' };
	ldr.createNewEntry(ed);

	ldr.save("Test.out");
}

TEST(Engine, LoaderLoadTest) {

	TestManager tm;
	Load::EditLoader ldr(tm);

	ldr.openFile("Loading/Test.in");

	Asset::EntryData ed = ldr.loadEntry(1);

	if (ed.entryId != 1) {
		FAIL();
	}
	if (ed.tag != Asset::Tag({ 'M', 'A', 'P', ' ' })) {
		FAIL();
	}
	if (ed.size != 10) {
		FAIL();
	}

	char data[10];
	memset(data, 'A', sizeof(data));

	if (memcmp(ed.data, data, sizeof(data)) != 0) {
		FAIL();
	}

}