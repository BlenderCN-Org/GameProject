#include "AddDataExtension.hpp"
#include <Editor_Wrapper\IEditor.hpp>

#include "../CoreGlobals.hpp"
#include "../System/Console.hpp"
#include "../AssetManager.hpp"

void AddDataExtension::execute(int nrArgs, ExtensionAddItemEvent * args)
{
	if (args != nullptr)
	{
		const char* name = args->objectData->getName();
		uint32_t formID = args->objectData->getFormID();
		void* data = args->objectData->getData();
		uint32_t dataSize = args->objectData->dataSize();

		AssetManager* assetMan = gAssetManager;

		if (args->objectType == OBJECT_TYPE_SCENE)
		{
			gConsole->print("Trying to add a scene\n");

			assetMan->createNewEntry(formID, SCENE_TAG);
		}
		else if(args->objectType == OBJECT_TYPE_RENDERLAYER) {
			gConsole->print("Trying to add a renderLayer\n");

			assetMan->createNewEntry(formID, RENDERLAYER_TAG);
		}
		else {
			gConsole->print("Trying to add a something else\n");
		}
	}
}

IExtension<void>* AddDataExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}
