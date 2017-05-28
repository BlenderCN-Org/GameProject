#include "GetFormIDExtension.hpp"

#include "../Assets/AssetManager.hpp"

#include <iostream>

void GetFormIDExtension::execute(int nrArgs, ExtensionGetFormIDEvent* arg)
{
	std::cout << "Execute Extention!" << std::endl;
	MasterFile* mFile = &AssetManager::getAssetManager()->masterFile;
	if (mFile->isIDTaken(arg->formID) == true)
	{
		arg->formID = mFile->getNextFormID();
	}
}

IExtension<void>* GetFormIDExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}