#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

#include <cstdint>

struct ExtensionSaveEvent {
	char* fileName;
	bool pack;
};

class IObject
{
public:
	virtual const char* getName() = 0;
	virtual const int getFormID() = 0;
	virtual void* getData() = 0;
};

struct ExtensionQueryDataEvent {
	uint32_t objectType;
	uint32_t nrObjects;
	IObject** objectList;
};

struct ExtensionAddItemEvent {
	uint32_t objectType;
	IObject* objectData;
};

struct ExtensionDeleteItemEvent {
	uint32_t formID;
};

struct ExtensionEditItemEvent {
	IObject* objectData;
};

template<typename T>
class IExtension {
public:
	virtual void execute(int nrArgs, T* arg) = 0;

	virtual IExtension<void>* toExtensionPtr() = 0;

};

#endif