#include "ExtensionHandler.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit)
{
	// unregsiter using nullptr
	edit->registerExtension(SAVE_CALLBACK, nullptr);
	edit->registerExtension(GET_OBJECTS_CALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;
	delete queryDataExtension;

	// clear handles;
	masterFileExtension = nullptr;
	queryDataExtension = nullptr;

	canInitialize = true;
}

void ExtensionHandler::loadExtensions(IEditor* edit)
{
	// safe check if we have initialized extensions already
	if (!canInitialize) return;

	// allocate extensions
	masterFileExtension = new MasterFileExtension();
	queryDataExtension = new QueryDataExtension();

	// register extensions
	edit->registerExtension(SAVE_CALLBACK, (IExtension<void>*)masterFileExtension);
	edit->registerExtension(GET_OBJECTS_CALLBACK, (IExtension<void>*)queryDataExtension);

	canInitialize = false;
}
