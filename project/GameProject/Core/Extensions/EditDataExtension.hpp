#ifndef EDITDATAEXTENSION_HPP
#define EDITDATAEXTENSION_HPP

#include "../../IExtension.hpp"

class EditDataExtension : public IExtension<ExtensionEditItemEvent> {

public:
	virtual void execute(int nrArgs, ExtensionEditItemEvent * arg) override;
	virtual IExtension<void>* toExtensionPtr() override;
};

#endif