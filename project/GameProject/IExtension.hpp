#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

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
	int objectType;
	int nrObjects;
	IObject** objectList;
};

struct ExtensionAddItemEvent {

};

template<typename T>
class IExtension {
public:
	virtual void execute(int nrArgs, T* arg) = 0;

	virtual IExtension<void>* toExtensionPtr() = 0;

};

#endif