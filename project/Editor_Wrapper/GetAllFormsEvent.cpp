#include "ExtensionMap.hpp"
#include <iostream>

namespace Extensions {
	void OnGetFormView(System::Object^ sender, Editor::EventHandler::FormView^ view) {
		//std::cout << "GetFormID Event\n";

		//view->totalEntryCount = assetMan->getNumEntries();
		//EntryMap entries = assetMan->getAllEntries();

		//EntryMap::iterator it = entries.begin();
		//EntryMap::iterator eit = entries.end();

		//for (it; it != eit; it++) {
		//	uint32_t formID = it->second.formID;
		//	bool deleteFlag = it->second.deleteFlag;

		//	Editor::DataSources::ViewForm^ vf = gcnew Editor::DataSources::ViewForm(formID, "", gcnew System::String(it->second.tag, 0, 4));
		//	vf->Deleted = deleteFlag;
		//	view->ReturnList->Add(vf);
		//}
	}
}