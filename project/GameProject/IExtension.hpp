#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

struct ExtensionSaveEvent {
	char* fileName;
	bool pack;
};

struct ExtensionQueryDataEvent {
	int objectType;
	int nrObjects;
	void* objectList;
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