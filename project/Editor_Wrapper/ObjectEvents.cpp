#include "ExtensionMap.hpp"
#include <iostream>

using namespace System::Runtime::InteropServices;

namespace Extensions {
	void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs) {
		//std::cout << "AddEvent\n";

		uint32_t formID = addArgs->FormID;

		switch (addArgs->ObjectType) {
			case Editor::EventHandler::ObjectTypes::SCENE:
				assetMan->createNewEntry(addArgs->FormID, SCENE_TAG);
				break;
			case Editor::EventHandler::ObjectTypes::RENDERLAYER:
				assetMan->createNewEntry(addArgs->FormID, RENDERLAYER_TAG);
				break;
			case Editor::EventHandler::ObjectTypes::STATIC:
				assetMan->createNewEntry(addArgs->FormID, STATICOBJECT_TAG);
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

	void editScene(Editor::EventHandler::FormArgs^ editArgs) {
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
	}

	void editRenderLayer(Editor::EventHandler::FormArgs^ editArgs) {
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
			}
		}

		Marshal::FreeHGlobal(ptr);
	}

	void editStaticObj(Editor::EventHandler::FormArgs^ editArgs) {
		Editor::DataSources::StaticObject^ staticObj = (Editor::DataSources::StaticObject^)editArgs->Data;

		IntPtr name = Marshal::StringToHGlobalAnsi(staticObj->Name);
		const char* nameStr = static_cast<const char*>(name.ToPointer());

		IntPtr meshFile = Marshal::StringToHGlobalAnsi(staticObj->MeshFile);
		const char* meshFileStr = static_cast<const char*>(meshFile.ToPointer());

		uint32_t formID = editArgs->FormID;
		Entry* e = assetMan->getEntry(formID);
		if (e != nullptr) {
			IStaticObjectDataObject* dataObj = assetMan->getConverter()->asStaticObjectData(e->data);

			if (dataObj) {
				StaticObjectSaveData sData = {};

				sData.name = nameStr;
				sData.meshFile = meshFileStr;
				sData.useCollision = staticObj->UseCollision ? true : false;

				dataObj->setStaticObjectData(&sData);
				e->entrySize = sizeof(EntrySave) + dataObj->getDataSize();
			}
		}

		Marshal::FreeHGlobal(name);
		Marshal::FreeHGlobal(meshFile);
	}

	void OnEditEvent(System::Object^ sender, Editor::EventHandler::FormArgs^ editArgs) {
		if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::FAKE) {
			Entry* e = assetMan->getEntry(editArgs->FormID);
			if (e == nullptr) {
				return;
			}
			IGameDataObject::Type t = e->data->getType();
			if (t == IGameDataObject::Type::SCENE) {
				ISceneDataObject* dObj = assetMan->getConverter()->asSceneData(e->data);
				SceneSaveData* data = dObj->getSceneData();
				SceneSaveData cpy;
				memcpy(&cpy, data, sizeof(SceneSaveData));
				std::string name(data->name ? data->name : "");
				cpy.name = name.c_str();
				dObj->setSceneData(&cpy);
			}
			e->entrySize = sizeof(EntrySave) + e->data->getDataSize();
		} else if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::SCENE) {
			editScene(editArgs);
		} else if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::RENDERLAYER) {
			editRenderLayer(editArgs);
		} else if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::STATIC) {
			editStaticObj(editArgs);
		} else if (editArgs->ObjectType == Editor::EventHandler::ObjectTypes::STARTUPSCENE) {
			assetMan->setStartupSceneRef(editArgs->FormID);
		}
	}
}