#ifndef EXTENSIONMAP_HPP
#define EXTENSIONMAP_HPP

#include "IEditor.hpp"
#include "CoreInclude.hpp"
#include <map>

namespace Extensions {
	extern std::map<int, IExtension<void>*> extensionMap;
	extern IAssetManager* assetMan;
	extern IEditorAccess* editAccess;

	// save event
	void OnSaveEvent(System::Object^ sender, Editor::EventHandler::SaveEventArgs^ saveArgs);
	// query event
	void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs);

	// add/edit/delete event
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs);
	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs);
	void OnDeleteEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ deleteArgs);

	void OnGetFormIDEvent(System::Object^ sender, Editor::EventHandler::GetFormIDArgs^ idArgs);

	void OnGetFormView(System::Object^ sender, Editor::EventHandler::FormView^ view);

	void OnSceneChangeEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ view);

	void OnAddToSceneEvent(System::Object^ sender, System::UInt32 id);
}
#endif