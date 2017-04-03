#ifndef MASTERFILEEXTENSION_HPP
#define MASTERFILEEXTENSION_HPP

#include "../../IExtension.hpp"
#include "../Assets/MasterFile.hpp"


class MasterFileExtension : public IExtension<ExtensionSaveEvent> {

public:
	void saveMasterFile();
	virtual void execute(int nrArgs, ExtensionSaveEvent* args);

private:

};


#endif
