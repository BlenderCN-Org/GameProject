#include "QueryDataExtension.hpp"

#include <Editor_Wrapper\IEditor.hpp>

void QueryDataExtension::execute(int nrArgs, ExtensionQueryDataEvent* args) {

	if (nrArgs != 1) {
		return;
	}

	if (args->objectType == OBJECT_TYPE_SCRIPT) {

		if (args->nrObjects == 0 && args->objectList == nullptr) {
			args->nrObjects = 2;
		}
		else {
			char** charArray = (char**)args->objectList;
			for (int i = 0; i < args->nrObjects; i++) {
				(char*)charArray[i] = "Test";
			}
		}
	}

}
