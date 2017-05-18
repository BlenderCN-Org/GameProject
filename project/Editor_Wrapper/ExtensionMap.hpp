#ifndef EXTENSIONMAP_HPP
#define EXTENSIONMAP_HPP

#include "IEditor.hpp"
#include "CoreInclude.hpp"
#include <map>

namespace Extensions
{
	extern std::map<int, IExtension<void>*> extensionMap;

	// save event
	void OnSaveEvent(System::Object^ sender, Editor::EventHandler::SaveEventArgs^ saveArgs);
	// query event
	void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs);
	
	// add/edit/delete event
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs);
	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs);
	void OnDeleteEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ deleteArgs);


}
#endif