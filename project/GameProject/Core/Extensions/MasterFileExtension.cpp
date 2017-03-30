#include "MasterFileExtension.hpp"
#include "../Assets/AssetManager.hpp"

#include <iostream>
#include "../System/Sys.hpp"

void MasterFileExtension::saveMasterFile() {

	MasterFile* mstFile = &AssetManager::getAssetManager()->masterFile;

	const char* fileName = mstFile->fileName;
	std::string tempName = std::string(fileName) + std::to_string((rand() * rand()));

	std::ofstream tempFile(tempName, std::ios::binary);

	MasterFileHeader hdr{};
	hdr.tag[0] = 'M';
	hdr.tag[1] = 'A';
	hdr.tag[2] = 'S';
	hdr.tag[3] = 'T';

	hdr.version.major = 1;
	hdr.version.major = 0;

	tempFile.write((char*)&hdr, sizeof(hdr));

	tempFile.close();

	// check if our new file is ok then delete and replace the old masterFile
	int err = 0;

	mstFile->masterFileHandle.close();
	err = remove(mstFile->fileName);
	if (err && fileExists(mstFile->fileName) ) {
		perror("Error deleting file\n");
	}

	err = rename(tempName.c_str(), mstFile->fileName);
	if (err) {
		printf("Error renaming file!\n");
		printf("Aborting Save!\n");
	}

	int b = 0;

}

void MasterFileExtension::execute(int nrArgs, void ** args)
{
	printf("MasterFileExtension -> Execute!\n");
	saveMasterFile();
}
