#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

#include <cstdint>

#include "FileStructs.hpp"

// callback defines
#define SAVE_CALLBACK              0x00000001

struct ExtensionSaveEvent {
	char* fileName;
	bool pack;
};

template<typename T>
class IExtension {
public:
	virtual void execute(int nrArgs, T* arg) = 0;

	virtual IExtension<void>* toExtensionPtr() = 0;
};

#endif