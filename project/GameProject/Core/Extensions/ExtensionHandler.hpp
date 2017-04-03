#ifndef EXTENSIONHANDLER_HPP
#define EXTENSIONHANDLER_HPP

#include "MasterFileExtension.hpp"
#include "QueryDataExtension.hpp"
#include <Editor_Wrapper\IEditor.hpp>

class ExtensionHandler
{
public:

	void unloadExtension(IEditor* edit);
	void loadExtensions(IEditor* edit);

	MasterFileExtension* masterFileExtension = nullptr;
	QueryDataExtension* queryDataExtension = nullptr;

private:
	bool canInitialize = true;

};

#endif