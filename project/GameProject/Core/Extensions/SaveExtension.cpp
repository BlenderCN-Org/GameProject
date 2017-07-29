#include "SaveExtension.hpp"

#include "../Assets/GameFile.hpp"

#include <iostream>
#include <string>

#include "../System/Sys.hpp"
#include "../CoreGlobals.hpp"
#include "../System/Console.hpp"

#include "../AssetManager.hpp"

void SaveExtension::saveMasterFile() {

	GameFile* gameFile = gAssetManager->currentFile;

	const char* fileName = gameFile->fileName;
	std::string tempName = std::string(fileName) + std::to_string((rand() * rand()));

	std::ofstream tempFile(tempName, std::ios::binary);

	GameFileHeader_V2 hdr{};

	hdr.fileVersion.version = 2;
	hdr.flags = 0;
	hdr.headerLength = sizeof(GameFileHeader_V2);
	hdr.nrEntries = gameFile->nrEntries;
	hdr.startupSceneRef = gAssetManager->getStartupSceneRef();

	tempFile.write((char*)&hdr, sizeof(hdr));

	gameFile->loadAllMissingEntries();

	EntryMap::const_iterator it = gameFile->loadedEntries.begin();
	EntryMap::const_iterator eit = gameFile->loadedEntries.end();

	uint32_t offset = sizeof(GameFileHeader_V2);
	offset += (2 * sizeof(uint32_t)) * hdr.nrEntries;

	OffsetTable of;

	for (it; it != eit; it++) {
		of[it->first] = offset;
		offset += it->second.entrySize;
	}

	it = gameFile->loadedEntries.begin();

	
	OffsetTable::const_iterator ofIt = of.begin();
	OffsetTable::const_iterator ofeIt = of.end();

	for (ofIt ; ofIt != ofeIt; ofIt++) {
		tempFile.write((char*)&ofIt->first, sizeof(uint32_t));
		tempFile.write((char*)&ofIt->second, sizeof(uint32_t));
	}

	for (it; it != eit; it++) {
		const Entry* e = &it->second;
		tempFile.write((char*)e, sizeof(EntrySave));
		tempFile.write((char*)e->data, e->entrySize - sizeof(EntrySave));
	}

	tempFile.close();

	// check if our new file is ok then delete and replace the old masterFile
	int err = 0;

	gameFile->inFile.close();
	err = remove(gameFile->fileName);
	if (err && fileExists(gameFile->fileName)) {
		gConsole->print("Error deleting file\n");
	}

	err = rename(tempName.c_str(), gameFile->fileName);
	if (err) {
		gConsole->print("Error renaming file!\n");
		gConsole->print("Aborting Save!\n");
	}

	gameFile->inFile.open(gameFile->fileName, std::ios::binary);

}

void SaveExtension::execute(int nrArgs, ExtensionSaveEvent* args) {
	gConsole->print("MasterFileExtension -> Execute!\n");
	saveMasterFile();
}

IExtension<void>* SaveExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
