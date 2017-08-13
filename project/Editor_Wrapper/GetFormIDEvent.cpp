#include "ExtensionMap.hpp"
#include <iostream>

namespace Extensions
{

	void OnGetFormIDEvent(System::Object^ sender, Editor::EventHandler::GetFormIDArgs^ idArgs)
	{
		//std::cout << "GetFormID Event\n";

		idArgs->FormID = assetMan->getNextFormID();

	}

}