#ifndef DELETEDATAEXTENSION_HPP
#define DELETEDATAEXTENSION_HPP

#include "../../IExtension.hpp"

class DeleteDataExtension : public IExtension<ExtensionDeleteItemEvent> {
public:
	// Inherited via IExtension
	virtual void execute(int nrArgs, ExtensionDeleteItemEvent * arg) override;
	virtual IExtension<void>* toExtensionPtr() override;
};

#endif