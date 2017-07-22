#include "GameFileEditor.hpp"

#include <AssetLib/AssetLib.hpp>

void GameFileEditor::init() {}

void GameFileEditor::load(const char * name) {

	fileName = name;

	uint32_t dataSize = 0;
	void* data = AssetLib::fileToMemory(name, dataSize);

	if (data) {
		Version* v = (Version*)data;

		if (v->version == 1) {
			GameFileHeader_V1* gameFileV1 = (GameFileHeader_V1*)data;

			nrEntries = gameFileV1->nrEntries;
			EntryOffset* offset = (EntryOffset*)((char*)data)[sizeof(GameFileHeader_V1)];
			for (uint32_t i = 0; i < nrEntries; i++) {
				offsetTable[offset[i].formID] = offset[i].offset;

				char* eData = (char*)data;

				Entry e = *(Entry*)eData[offset[i].offset];
				
				uint32_t dataSize = e.entrySize - sizeof(Entry);
				e.data = new char[dataSize];

				memcpy(e.data, &eData[offset[i].offset + sizeof(Entry)], dataSize);

				loadedEntries[offset[i].formID] = e;

			}

		}

		delete[] data;

		inFile.open(name, std::ios::binary);

	} else {
		// failed to read file
	}

}

