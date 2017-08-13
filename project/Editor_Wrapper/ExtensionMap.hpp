#ifndef EXTENSIONMAP_HPP
#define EXTENSIONMAP_HPP

#include "IEditor.hpp"
#include "CoreInclude.hpp"
#include <map>

namespace Extensions
{

	class AddObject : public IObject
	{
	public:
		// Inherited via IObject
		virtual const char * getName() override;
		virtual const uint32_t getFormID() override;
		virtual void * getData() override;
		virtual uint32_t dataSize() override;
		virtual bool isDeleted() override;

		const char* name;
		uint32_t formID;
		void* data;
		uint32_t sizeize;
	};

	extern std::map<int, IExtension<void>*> extensionMap;
	extern IAssetManager* assetMan;

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

}
#endif