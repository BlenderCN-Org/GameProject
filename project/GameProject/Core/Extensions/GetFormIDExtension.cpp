#include "GetFormIDExtension.hpp"

#include "../CoreGlobals.hpp"
#include "../AssetManager.hpp"

#include <iostream>

void GetFormIDExtension::execute(int nrArgs, ExtensionGetFormIDEvent* arg)
{
	std::cout << "Execute Extention!" << std::endl;
	arg->formID = gAssetManager->getNextFormID();
}

IExtension<void>* GetFormIDExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}