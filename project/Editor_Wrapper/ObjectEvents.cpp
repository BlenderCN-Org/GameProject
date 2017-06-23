#include "ExtensionMap.hpp"
#include <iostream>	

using namespace System::Runtime::InteropServices;

class EditObject : public IObject{
	
public:
	virtual const char * getName() override {
		return name;
	}
	virtual const uint32_t getFormID() override {
		return formID;
	}
	virtual void * getData() override {
		return data;
	}
	virtual uint32_t dataSize() override {
		return 0;
	}
	virtual bool isDeleted() override {
		return false;
	}

	const char* name;
	uint32_t formID;
	void* data;

};

namespace Extensions
{
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs)
	{
		std::cout << "AddEvent\n";

		if (extensionMap.count(ADD_OBJECT_CALLBACK) >= 1 && extensionMap[ADD_OBJECT_CALLBACK]) {

			int type = (int)addArgs->ObjectType;

			AddObject add;
			add.formID = addArgs->FormID;
			//add.name = addArgs->Name;
			IntPtr ptr = Marshal::StringToHGlobalAnsi(addArgs->Name);
			const char* str = static_cast<const char*>(ptr.ToPointer());
			add.name = str;

			ExtensionAddItemEvent addEvnt;
			addEvnt.objectData = &add;
			addEvnt.objectType = type;
			extensionMap[ADD_OBJECT_CALLBACK]->execute(1, &addEvnt);
			Marshal::FreeHGlobal(ptr);
		}
	}

	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs)
	{
		std::cout << "EditEvent\n";
		if (extensionMap.count(EDIT_OBJECT_CALLBACK) >= 1 && extensionMap[EDIT_OBJECT_CALLBACK]) {

			ExtensionEditItemEvent evnt;

			EditObject* obj = new EditObject();

			obj->formID = editArgs->FormID;
			Editor::DataSources::Scene^ scene = (Editor::DataSources::Scene^)editArgs->Data;
			SceneStuff* sc = new SceneStuff();
			IntPtr ptr = Marshal::StringToHGlobalAnsi(scene->Name);
			const char* str = static_cast<const char*>(ptr.ToPointer());
			obj->name = str;

			sc->skyColor[0] = float(scene->SkyColor.R) / 255.0f;
			sc->skyColor[1] = float(scene->SkyColor.G) / 255.0f;
			sc->skyColor[2] = float(scene->SkyColor.B) / 255.0f;
			sc->skyColor[3] = float(scene->SkyColor.A) / 255.0f;
			sc->hasFog = scene->HasFog ? true : false;

			sc->fog[0] = float(scene->FogColorNear.R) / 255.0f;
			sc->fog[1] = float(scene->FogColorNear.G) / 255.0f;
			sc->fog[2] = float(scene->FogColorNear.B) / 255.0f;
			sc->fog[3] = float(scene->FogColorNear.A) / 255.0f;

			sc->fog[4] = float(scene->FogColorFar.R) / 255.0f;
			sc->fog[5] = float(scene->FogColorFar.G) / 255.0f;
			sc->fog[6] = float(scene->FogColorFar.B) / 255.0f;
			sc->fog[7] = float(scene->FogColorFar.A) / 255.0f;

			sc->hasWater = scene->HasWater ? true : false;

			obj->data = sc;
			evnt.objectData = obj;
			extensionMap[EDIT_OBJECT_CALLBACK]->execute(1, &evnt);

			Marshal::FreeHGlobal(ptr);
			delete sc;
			delete obj;
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