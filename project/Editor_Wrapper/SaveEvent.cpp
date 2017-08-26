#include "ExtensionMap.hpp"
#include <iostream>

namespace Extensions {
	void OnSaveEvent(System::Object^ sender, Editor::EventHandler::SaveEventArgs^ saveArgs) {
		std::cout << "SaveEvent\n";
		Console::Write(": ");
		Console::WriteLine(saveArgs->PackData);

		ExtensionSaveEvent extSave;
		extSave.fileName = "blabla";
		extSave.pack = saveArgs->PackData;
		if (Extensions::extensionMap.count(SAVE_CALLBACK) >= 1 && Extensions::extensionMap[SAVE_CALLBACK]) {
			Extensions::extensionMap[SAVE_CALLBACK]->execute(1, &extSave);
		}
	}
}