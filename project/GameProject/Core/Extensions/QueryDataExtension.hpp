#ifndef QUERYDATAXTENSION_HPP
#define QUERYDATAXTENSION_HPP

#include "../../IExtension.hpp"

class QueryDataExtension : public IExtension<ExtensionQueryDataEvent> {

public:
	virtual void execute(int nrArgs, ExtensionQueryDataEvent* args);

	// Inherited via IExtension
	virtual IExtension<void>* toExtensionPtr() override;

};


#endif