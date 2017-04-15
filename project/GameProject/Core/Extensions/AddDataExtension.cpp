#include "AddDataExtension.hpp"

void AddDataExtension::execute(int nrArgs, ExtensionAddItemEvent * args)
{
}

IExtension<void>* AddDataExtension::toExtensionPtr()
{
	return (IExtension<void>*)this;
}
