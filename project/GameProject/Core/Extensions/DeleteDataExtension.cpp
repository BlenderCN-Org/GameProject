#include "DeleteDataExtension.hpp"

#include "../Assets/AssetManager.hpp"
#include "../CoreGlobals.hpp"
#include "../Core.hpp"

void DeleteDataExtension::execute(int nrArgs, ExtensionDeleteItemEvent * arg) {
	if (arg != nullptr && nrArgs == 1) {
		AssetManager::getAssetManager()->masterFile.entries[arg->formID].deleted = true;
		g_core->getConsole()->print("Deleting Form: " + std::to_string(arg->formID) + "\n" );
	}
}

IExtension<void>* DeleteDataExtension::toExtensionPtr() {
	return (IExtension<void>*)this;
}
