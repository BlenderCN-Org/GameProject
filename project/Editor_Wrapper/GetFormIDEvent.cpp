#include "ExtensionMap.hpp"
#include <iostream>

namespace Extensions
{

	void OnGetFormIDEvent(System::Object^ sender, Editor::EventHandler::GetFormIDArgs^ idArgs)
	{
		std::cout << "GetFormID Event\n";

		if (extensionMap.count(GET_FORMID_CALLBACK) >= 1 && extensionMap[GET_FORMID_CALLBACK]) {

			ExtensionGetFormIDEvent formIdArgs{};
			formIdArgs.formID = 0;

			extensionMap[GET_FORMID_CALLBACK]->execute(1, &formIdArgs);

			idArgs->FormID = formIdArgs.formID;

		}
	}

}