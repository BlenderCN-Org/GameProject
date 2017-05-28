#include "AddDataExtension.hpp"
#include <Editor_Wrapper\IEditor.hpp>
#include "../Assets/AssetManager.hpp"

void AddDataExtension::execute(int nrArgs, ExtensionAddItemEvent * args)
{
	if (args != nullptr)
	{
		const char* name = args->objectData->getName();
		uint32_t formID = args->objectData->getFormID();
		void* data = args->objectData->getData();
		uint32_t dataSize = args->objectData->dataSize();

		AssetManager* assetMan = AssetManager::getAssetManager();

		if (args->objectType == OBJECT_TYPE_SCENE)
		{
			assetMan->addScene(name, formID, data, dataSize);

		}
	}
}

IExtension<void>* AddDataExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}
