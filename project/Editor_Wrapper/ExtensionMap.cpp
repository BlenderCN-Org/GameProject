#include "ExtensionMap.hpp"

#include <iostream>

std::map<int, IExtension<void>*> Extensions::extensionMap;

namespace Extensions
{
	const char * AddObject::getName()
	{
		return name;
	}
	const uint32_t AddObject::getFormID()
	{
		return formID;
	}
	void * AddObject::getData()
	{
		return data;
	}
	uint32_t AddObject::dataSize()
	{
		return sizeize;
	}
}