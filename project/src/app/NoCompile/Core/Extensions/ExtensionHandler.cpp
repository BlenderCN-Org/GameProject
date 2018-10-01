#include "ExtensionHandler.hpp"
#include "../CoreGlobals.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit) {
	// unregsiter using nullptr
	edit->registerExtension(SAVE_CALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;

	// clear handles;
	masterFileExtension = nullptr;

	canInitialize = true;
}

void ExtensionHandler::loadExtensions(IEditor* edit) {
	// safe check if we have initialized extensions already
	if (!canInitialize) return;

	// allocate extensions
	masterFileExtension = new SaveExtension();

	// register extensions
	edit->registerExtension(SAVE_CALLBACK, masterFileExtension->toExtensionPtr());

	canInitialize = false;
}