#include "ExtensionHandler.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit)
{
	// unregsiter using nullptr
	edit->registerExtension(SAVE_CALLBACK, nullptr);
	edit->registerExtension(GET_OBJECTS_CALLBACK, nullptr);
	edit->registerExtension(ADD_OBJECT_CALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;
	delete queryDataExtension;
	delete addDataExtension;

	// clear handles;
	masterFileExtension = nullptr;
	queryDataExtension = nullptr;
	addDataExtension = nullptr;

	canInitialize = true;
}

void ExtensionHandler::loadExtensions(IEditor* edit)
{
	// safe check if we have initialized extensions already
	if (!canInitialize) return;

	// allocate extensions
	masterFileExtension = new MasterFileExtension();
	queryDataExtension = new QueryDataExtension();
	addDataExtension = new AddDataExtension();

	// register extensions
	edit->registerExtension(SAVE_CALLBACK, masterFileExtension->toExtensionPtr());
	edit->registerExtension(GET_OBJECTS_CALLBACK, queryDataExtension->toExtensionPtr());
	edit->registerExtension(ADD_OBJECT_CALLBACK, addDataExtension->toExtensionPtr());

	canInitialize = false;
}
