#include "MasterFile.hpp"

MasterFile::MasterFile() {}

MasterFile::~MasterFile() {}

bool MasterFile::loadMaster(const char * masterFileName) {

	bool status = true;

	masterFileHandle.open(masterFileName);
	status = masterFileHandle.is_open();

	if ( status ) {

		MasterFileHeader mfh{ 0 };

		masterFileHandle.read( (char*)&mfh, sizeof(mfh));

		int* tag = (int*)mfh.tag;
		int* masterTag = (int*)"MAST";

		if ( *tag != *masterTag ) {
			printf("Invalid masterFile: \n");
			printf("\t Invalid header tag! \n");
			status = false;
		}

	} else {
		printf("MasterFile not found!\n");
	}

	return status;
}
