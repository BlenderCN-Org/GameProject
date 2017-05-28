#include "MasterFile.hpp"

#include "Forms\MenuForm.hpp"
#include "Forms\SceneForm.hpp"
#include "../CoreGlobals.hpp"
#include "../Core.hpp"

MasterFile::MasterFile() : nextID(1) {
	registerForms();
}

MasterFile::~MasterFile() {
	for (FormEntries::const_iterator it = entries.begin(); it != entries.end(); it++) {
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
		for (uint32_t i = 0; i < mfh.preloadEntries; i++) {
			// do preloading stuff
		}

		// load forms
		for (uint32_t i = 0; i < mfh.entries; i++) {
			FormHeader frmHdr{ };
			masterFileHandle.read((char*)&frmHdr, sizeof(frmHdr));
			void* data = malloc(frmHdr.formSize - sizeof(frmHdr));
			masterFileHandle.read((char*)data, frmHdr.formSize - sizeof(frmHdr));
			FormEntry entry;
			entry.deleted = false;
			entry.form = BaseForm::createFromHeader(frmHdr, data);

			if (entry.form != nullptr) {
				entries[frmHdr.formID] = entry;
				//typeMapper.insert(std::pair<uint32_t, uint32_t>(entry.form->getType(), frmHdr.formID));
				typeMapper[entry.form->getType()].insert(frmHdr.formID);
			} else {
				// notify invalid header and that object is getting deleted
				g_core->getConsole()->print("Error loading object: " + std::to_string(frmHdr.formID) + "\n" );
			}
			free(data);
		}

		// load named entries table
		NamedEntry* namedOffsets = new NamedEntry[mfh.namedEntries];
		masterFileHandle.read((char*)namedOffsets, sizeof(NamedEntry) * mfh.namedEntries);
		for (uint32_t i = 0; i < mfh.namedEntries; i++) {
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

	} else {
		printf("MasterFile not found!\n");
	}

	return status;
}

BaseForm* MasterFile::getForm(const uint32_t formID) {
	return entries[formID].form;
}

bool MasterFile::isFormPresent(uint32_t formID) {
	if (offsetTable.count(formID) > 0)
		return true;
	return false;
}

bool MasterFile::isFormDeleted(const uint32_t formID) {
	return entries[formID].deleted;
}

uint32_t MasterFile::getFormIDFromNamedSection(const char * name) {
	std::string namedItem = std::string(name);
	if (namedEntriesTable.count(namedItem) > 0) {
		return namedEntriesTable.at(namedItem);
	}
	return uint32_t();
}

bool MasterFile::isIDTaken(uint32_t formID) {
	return entries.count(formID) == 1;
}


uint32_t MasterFile::getNextFormID() {
	while (entries.count(nextID) != 0) {
		nextID++;
	}
	return nextID;
}

void MasterFile::addForm(uint32_t type, BaseForm* form, uint32_t formID) {
	FormEntry fe;
	fe.form = form;
	fe.deleted = false;
	entries[formID] = fe;
	//typeMapper.insert(std::pair<uint32_t, uint32_t>(type, formID));
	typeMapper[type].insert(formID);
}

uint32_t MasterFile::getNumberUsingType(uint32_t type) {
	
	if (typeMapper.count(type) != 0) {
		return typeMapper[type].size();
	}
	return 0;
}

uint32_t MasterFile::getFormIDFromTypeAndIndex(uint32_t type, uint32_t index) {
	
	uint32_t id = 0;
	if (typeMapper.count(type) != 0) {

		std::set<uint32_t>::iterator it = typeMapper[type].begin();
		std::advance(it, index);
		id = *it;
	}

	return id;
}

