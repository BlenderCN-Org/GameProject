#include "ExtensionMap.hpp"
#include <iostream>	

namespace Extensions
{
	
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs)
	{
		std::cout << "AddEvent\n";

		if (extensionMap.count(ADD_OBJECT_CALLBACK) >= 1 && extensionMap[ADD_OBJECT_CALLBACK]) {

			int type = (int)addArgs->ObjectType;

			extensionMap[ADD_OBJECT_CALLBACK]->execute(0, nullptr);

		}
	}

	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs)
	{
		std::cout << "EditEvent\n";
		if (extensionMap.count(EDIT_OBJECT_CALLBACK) >= 1 && extensionMap[EDIT_OBJECT_CALLBACK]) {

			extensionMap[EDIT_OBJECT_CALLBACK]->execute(0, nullptr);
		}
	}

	void OnDeleteEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ deleteArgs)
	{
		std::cout << "Delete\n";

		if (extensionMap.count(DELETE_OBJECT_CALLBACK) >= 1 && extensionMap[DELETE_OBJECT_CALLBACK]) {

			uint32_t deleteForm = (uint32_t)deleteArgs->FormID;

			ExtensionDeleteItemEvent deleteEvent;
			deleteEvent.formID = deleteForm;

			extensionMap[DELETE_OBJECT_CALLBACK]->execute(1, &deleteEvent);
		}
	}

}