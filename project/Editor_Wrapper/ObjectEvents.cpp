#include "ExtensionMap.hpp"
#include <iostream>	

using namespace System::Runtime::InteropServices;

class EditObject : public IObject {

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

namespace Extensions {
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs) {
		//std::cout << "AddEvent\n";

		uint32_t type = (uint32_t)addArgs->ObjectType;

		uint32_t formID = addArgs->FormID;

		switch (type) {
			case OBJECT_TYPE_SCENE:
				assetMan->createNewEntry(addArgs->FormID, SCENE_TAG);
				break;
			case OBJECT_TYPE_RENDERLAYER:
				assetMan->createNewEntry(addArgs->FormID, RENDERLAYER_TAG);
				break;
			default:
				break;
		}
	}

	void OnDeleteEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ deleteArgs) {
		//std::cout << "Delete\n";
		Entry* e = assetMan->getEntry((uint32_t)deleteArgs->FormID);
		e->deleteFlag = !e->deleteFlag;
		assetMan->updateEntry(e);

	}

	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs) {
		//std::cout << "EditEvent\n";
		if (extensionMap.count(EDIT_OBJECT_CALLBACK) >= 1 && extensionMap[EDIT_OBJECT_CALLBACK]) {

			ExtensionEditItemEvent evnt;

			EditObject* obj = new EditObject();

			obj->formID = editArgs->FormID;
			obj->data = nullptr;
			evnt.objectType = (uint32_t)editArgs->ObjectType;

			if (evnt.objectType == 0) {
				evnt.objectData = obj;
				extensionMap[EDIT_OBJECT_CALLBACK]->execute(1, &evnt);
			}

			if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::SCENE) {
				Editor::DataSources::Scene^ scene = (Editor::DataSources::Scene^)editArgs->Data;
				IntPtr ptr = Marshal::StringToHGlobalAnsi(scene->Name);
				const char* str = static_cast<const char*>(ptr.ToPointer());

				uint32_t formID = editArgs->FormID;
				Entry* e = assetMan->getEntry(formID);

				if (e != nullptr) {
					ISceneDataObject* dataObj = assetMan->getConverter()->asSceneData(e->data);

					if (dataObj) {
						SceneSaveData sData = {};

						sData.name = str;
						sData.skyColor[0] = float(scene->SkyColor.R) / 255.0f;
						sData.skyColor[1] = float(scene->SkyColor.G) / 255.0f;
						sData.skyColor[2] = float(scene->SkyColor.B) / 255.0f;
						sData.skyColor[3] = float(scene->SkyColor.A) / 255.0f;

						sData.hasFog = scene->HasFog ? true : false;

						sData.fog[0] = float(scene->FogColorNear.R) / 255.0f;
						sData.fog[1] = float(scene->FogColorNear.G) / 255.0f;
						sData.fog[2] = float(scene->FogColorNear.B) / 255.0f;
						sData.fog[3] = float(scene->FogColorNear.A) / 255.0f;

						sData.fog[4] = float(scene->FogColorFar.R) / 255.0f;
						sData.fog[5] = float(scene->FogColorFar.G) / 255.0f;
						sData.fog[6] = float(scene->FogColorFar.B) / 255.0f;
						sData.fog[7] = float(scene->FogColorFar.A) / 255.0f;

						sData.hasWater = scene->HasWater ? true : false;

						dataObj->setSceneData(&sData);

						e->entrySize = sizeof(EntrySave) + dataObj->getDataSize();

					}
				}
				Marshal::FreeHGlobal(ptr);

			} else if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::RENDERLAYER) {
				//Editor::DataSources::RenderLayer^ renderLayer = (Editor::DataSources::RenderLayer^)editArgs->Data;
				//RenderLayerSaveData* rData = new RenderLayerSaveData();
				//IntPtr ptr = Marshal::StringToHGlobalAnsi(renderLayer->Name);
				//const char* str = static_cast<const char*>(ptr.ToPointer());
				//
				//rData->name = str;
				//rData->resolutionType = renderLayer->ResolutionType;
				//rData->height = renderLayer->ResolutionHeight;
				//rData->width = renderLayer->ResolutionWidth;
				//rData->depthBuffer = renderLayer->DepthBuffer;
				//rData->stencilBuffer = renderLayer->StencilBuffer;
				//rData->nrColorBuffers = renderLayer->NumColorAttachments;
				//rData->shaderProgramRef = renderLayer->ShaderProgram;
				//
				//obj->data = rData;
				//evnt.objectData = obj;
				//
				//extensionMap[EDIT_OBJECT_CALLBACK]->execute(1, &evnt);
				//
				//Marshal::FreeHGlobal(ptr);
				//delete rData;

				Editor::DataSources::RenderLayer^ renderLayer = (Editor::DataSources::RenderLayer^)editArgs->Data;
				IntPtr ptr = Marshal::StringToHGlobalAnsi(renderLayer->Name);
				const char* str = static_cast<const char*>(ptr.ToPointer());

				uint32_t formID = editArgs->FormID;
				Entry* e = assetMan->getEntry(formID);

				if (e != nullptr) {
					IRenderLayerDataObject* dataObj = assetMan->getConverter()->asRenderLayer(e->data);

					if (dataObj) {
						RenderLayerSaveData sData = {};

						sData.name = str;
						sData.resolutionType = renderLayer->ResolutionType;
						sData.width = renderLayer->ResolutionWidth;
						sData.height = renderLayer->ResolutionWidth;
						sData.depthBuffer = renderLayer->DepthBuffer;
						sData.stencilBuffer = renderLayer->StencilBuffer;
						sData.nrColorBuffers = renderLayer->NumColorAttachments;
						sData.shaderProgramRef = renderLayer->ShaderProgram;

						dataObj->setRenderLayerData(&sData);

						e->entrySize = sizeof(EntrySave) + dataObj->getDataSize();
						printf("Edit\n");
					}
				}

				Marshal::FreeHGlobal(ptr);

			}

			delete obj;

		}
	}

}