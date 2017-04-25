#include "MasterFile.hpp"

#include "Forms\MenuForm.hpp"

MasterFile::MasterFile() {
	registerForms();
}

MasterFile::~MasterFile() {
	for (FormEntries::const_iterator it = entries.begin(); it != entries.end(); it++)
	{
		delete it->second.form;
	}
	entries.clear();
}

bool MasterFile::loadMaster(const char * masterFileName) {

	bool status = true;
	fileName = masterFileName;
	masterFileHandle.open(masterFileName);
	status = masterFileHandle.is_open();

	if (status) {

		MasterFileHeader mfh{ 0 };

		masterFileHandle.read((char*)&mfh, sizeof(mfh));

		int* tag = (int*)mfh.tag;
		int* masterTag = (int*)"MAST";

		if (*tag != *masterTag) {
			printf("Invalid masterFile: \n");
			printf("\t Invalid header tag! \n");
			status = false;
		}

		// load preloaded entries
		FormOffset* preloadOffsets = new FormOffset[mfh.preloadEntries];
		masterFileHandle.read((char*)preloadOffsets, sizeof(FormOffset) * mfh.preloadEntries);
		for (uint32_t i = 0; i < mfh.preloadEntries; i++)
		{
			// do preloading stuff
		}

		// load forms
		for (uint32_t i = 0; i < mfh.entries; i++)
		{
			FormHeader frmHdr { };
			masterFileHandle.read((char*)&frmHdr, sizeof(frmHdr));
			void* data = malloc(frmHdr.formSize - sizeof(frmHdr));
			masterFileHandle.read((char*)data, frmHdr.formSize - sizeof(frmHdr));
			FormEntry entry;
			entry.deleted = false;
			entry.form = BaseForm::createFromHeader(frmHdr, data);
			entries[frmHdr.formID] = entry;
			free(data);
		}

		// load named entries table
		NamedEntry* namedOffsets = new NamedEntry[mfh.namedEntries];
		masterFileHandle.read((char*)namedOffsets, sizeof(NamedEntry) * mfh.namedEntries);
		for (uint32_t i = 0; i < mfh.namedEntries; i++)
		{
			uint32_t formID = namedOffsets[i].formID;
			uint32_t offset = namedOffsets[i].offset;
			std::string name(namedOffsets[i].name);
			namedEntriesTable[name] = formID;
			offsetTable[formID] = offset;
		}

		delete[] namedOffsets;
		delete[] preloadOffsets;

		//	MenuForm* menu = new MenuForm();
		//	FormEntry entry;
		//	entry.deleted = false;
		//	entry.form = menu;
		//	entries[menu->getFormID()] = entry;

	}
	else {
		printf("MasterFile not found!\n");
	}

	return status;
}

BaseForm* MasterFile::getForm(const uint32_t formID)
{
	return entries[formID].form;
}

bool MasterFile::isFormPresent(uint32_t formID)
{
	if (offsetTable.count(formID) > 0)
		return true;
	return false;
}

uint32_t MasterFile::getFormIDFromNamedSection(const char * name)
{
	std::string namedItem = std::string(name);
	if (namedEntriesTable.count(namedItem) > 0) {
		return namedEntriesTable.at(namedItem);
	}
	return uint32_t();
}
