#ifndef EXTENSIONHANDLER_HPP
#define EXTENSIONHANDLER_HPP

#include <Editor_Wrapper\IEditor.hpp>

#include "SaveExtension.hpp"
#include "QueryDataExtension.hpp"
#include "AddDataExtension.hpp"
#include "GetFormIDExtension.hpp"
#include "DeleteDataExtension.hpp"
#include "EditDataExtension.hpp"

class ExtensionHandler
{
public:

	void unloadExtension(IEditor* edit);
	void loadExtensions(IEditor* edit);

	SaveExtension* masterFileExtension = nullptr;
	QueryDataExtension* queryDataExtension = nullptr;
	AddDataExtension* addDataExtension = nullptr;
	GetFormIDExtension* getFormIDExtension = nullptr;
	DeleteDataExtension* deleteDataExtension = nullptr;
	EditDataExtension* editDataExtension = nullptr;

private:
	bool canInitialize = true;

};

#endif