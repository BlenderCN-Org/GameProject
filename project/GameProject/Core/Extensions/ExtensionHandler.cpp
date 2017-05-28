#include "ExtensionHandler.hpp"

void ExtensionHandler::unloadExtension(IEditor* edit)
{
	// unregsiter using nullptr
	edit->registerExtension(SAVE_CALLBACK, nullptr);
	edit->registerExtension(GET_OBJECTS_CALLBACK, nullptr);
	edit->registerExtension(ADD_OBJECT_CALLBACK, nullptr);
	edit->registerExtension(GET_FORMID_CALLBACK, nullptr);
	edit->registerExtension(DELETE_OBJECT_CALLBACK, nullptr);
	edit->registerExtension(EDIT_OBJECT_CALLBACK, nullptr);

	// free extensions
	delete masterFileExtension;
	delete queryDataExtension;
	delete addDataExtension;
	delete getFormIDExtension;
	delete deleteDataExtension;
	delete editDataExtension;

	// clear handles;
	masterFileExtension = nullptr;
	queryDataExtension = nullptr;
	addDataExtension = nullptr;
	getFormIDExtension = nullptr;
	deleteDataExtension = nullptr;
	editDataExtension = nullptr;

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
	getFormIDExtension = new GetFormIDExtension();
	deleteDataExtension = new DeleteDataExtension();
	editDataExtension = new EditDataExtension();

	// register extensions
	edit->registerExtension(SAVE_CALLBACK, masterFileExtension->toExtensionPtr());
	edit->registerExtension(GET_OBJECTS_CALLBACK, queryDataExtension->toExtensionPtr());
	edit->registerExtension(ADD_OBJECT_CALLBACK, addDataExtension->toExtensionPtr());
	edit->registerExtension(GET_FORMID_CALLBACK, getFormIDExtension->toExtensionPtr());
	edit->registerExtension(DELETE_OBJECT_CALLBACK, deleteDataExtension->toExtensionPtr());
	edit->registerExtension(EDIT_OBJECT_CALLBACK, editDataExtension->toExtensionPtr());


	canInitialize = false;
}
