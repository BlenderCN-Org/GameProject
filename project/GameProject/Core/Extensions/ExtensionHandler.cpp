#include "ExtensionHandler.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit)
{
	// unregsiter using nullptr
	edit->registerExtension(SAVECALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;

	// clear handles;
	masterFileExtension = nullptr;
	canInitialize = true;
}

void ExtensionHandler::loadExtensions(IEditor* edit)
{
	// safe check if we have initialized extensions already
	if (!canInitialize) return;

	masterFileExtension = new MasterFileExtension();
	edit->registerExtension(SAVECALLBACK, masterFileExtension);

	canInitialize = false;
}
