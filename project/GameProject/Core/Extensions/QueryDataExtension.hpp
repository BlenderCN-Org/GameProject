#ifndef QUERYDATAXTENSION_HPP
#define QUERYDATAXTENSION_HPP

#include "../../IExtension.hpp"
#include "../Assets/MasterFile.hpp"


class QueryDataExtension : public IExtension<ExtensionQueryDataEvent> {

public:
	void queryData();
	//template<typename T>
	virtual void execute(int nrArgs, ExtensionQueryDataEvent* args);

	// Inherited via IExtension
	virtual IExtension<void>* toExtensionPtr() override;

};


#endif