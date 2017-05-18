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
	memcpy(hdr.tag, "MAST", 4);

	hdr.version.major = 1;
	hdr.version.major = 0;
	
	hdr.entries = (uint32_t)mstFile->entries.size();
	hdr.namedEntries = 0;
	hdr.preloadEntries = 0;

	// write header
	tempFile.write((char*)&hdr, sizeof(hdr));

	// write tables
	
	FormOffsetTable offsetTable = mstFile->offsetTable;


	// write forms
	for (FormEntries::const_iterator it = mstFile->entries.begin(); it != mstFile->entries.end(); it++ )
	{
		BaseForm* form = it->second.form;
		uint32_t size = form->getFormSize();
		void* data = malloc(size);
		form->getFormData(data);
		tempFile.write((char*)data, size);
		free(data);
	}
	//for (uint32_t i = 0; i < hdr.entries; i++)
	//{
	//	BaseForm* form = mstFile->entries.at(i).form;
	//	
	//	uint32_t size = form->getFormSize();
	//	void* data = malloc(size);
	//	form->getFormData(data);
	//	tempFile.write((char*)data, size);
	//}

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

void MasterFileExtension::execute(int nrArgs, ExtensionSaveEvent* args)
{
	printf("MasterFileExtension -> Execute!\n");
	saveMasterFile();
}

IExtension<void>* MasterFileExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}
