#ifndef EXTENSIONHANDLER_HPP
#define EXTENSIONHANDLER_HPP

#include <Editor_Wrapper\IEditor.hpp>

#include "SaveExtension.hpp"

class ExtensionHandler {
public:

	void unloadExtension(IEditor* edit);
	void loadExtensions(IEditor* edit);

	SaveExtension* masterFileExtension = nullptr;

private:
	bool canInitialize = true;
};

#endif