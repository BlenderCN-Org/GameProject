#ifndef GETFORMIDEXTENSION_HPP
#define GETFORMIDEXTENSION_HPP

#include "../../IExtension.hpp"

class GetFormIDExtension : public IExtension<ExtensionGetFormIDEvent>
{
public:
	// Inherited via IExtension
	virtual void execute(int nrArgs, ExtensionGetFormIDEvent * arg) override;
	virtual IExtension<void>* toExtensionPtr() override;
};

#endif