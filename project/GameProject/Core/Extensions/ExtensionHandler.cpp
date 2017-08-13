#include "ExtensionHandler.hpp"
#include "../CoreGlobals.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit) {
	// unregsiter using nullptr
	edit->registerExtension(SAVE_CALLBACK, nullptr);
	edit->registerExtension(EDIT_OBJECT_CALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;
	delete editDataExtension;

	// clear handles;
	masterFileExtension = nullptr;
	editDataExtension = nullptr;

	canInitialize = true;
}

void ExtensionHandler::loadExtensions(IEditor* edit) {
	// safe check if we have initialized extensions already
	if (!canInitialize) return;

	// allocate extensions
	masterFileExtension = new SaveExtension();
	editDataExtension = new EditDataExtension();

	// register extensions
	edit->registerExtension(SAVE_CALLBACK, masterFileExtension->toExtensionPtr());
	edit->registerExtension(EDIT_OBJECT_CALLBACK, editDataExtension->toExtensionPtr());
	
	canInitialize = false;
}
