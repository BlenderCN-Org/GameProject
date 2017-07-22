#ifndef ADDDATAEXTENSION_HPP
#define ADDDATAEXTENSION_HPP

#include "../../IExtension.hpp"


class AddDataExtension : public IExtension<ExtensionAddItemEvent> {

public:
	virtual void execute(int nrArgs, ExtensionAddItemEvent* args);
	virtual IExtension<void>* toExtensionPtr();
};

#endif