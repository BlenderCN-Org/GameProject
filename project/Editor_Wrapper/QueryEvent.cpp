#include "ExtensionMap.hpp"
#include <iostream>	

using namespace System::Runtime::InteropServices;

namespace Extensions {

	System::Windows::Media::Color^ toColor(float _r, float _g, float _b, float _a) {

		unsigned char r, g, b, a;

		r = (unsigned char)(_r * 255.0f);
		g = (unsigned char)(_g * 255.0f);
		b = (unsigned char)(_b * 255.0f);
		a = (unsigned char)(_a * 255.0f);

		return System::Windows::Media::Color::FromArgb(a, r, g, b);

	}

	void GetSceneData(Editor::EventHandler::QueryDataArgs^ queryArgs) {
		uint32_t tag = 0;
		memcpy(&tag, SCENE_TAG, 4);
		std::vector<uint32_t> objects = assetMan->getObjectsWithTag(tag);

		uint32_t nrObjects = objects.size();

		for (size_t i = 0; i < nrObjects; i++) {

			Entry* e = assetMan->getEntry(objects[i]);

			ISceneDataObject* sceneDataObj = nullptr;
			SceneSaveData* sData = nullptr;

			Editor::DataSources::Scene^ data = gcnew Editor::DataSources::Scene();
			System::String^ str = nullptr;

			if (e->data) {
				sceneDataObj = assetMan->getConverter()->asSceneData(e->data);

				sData = sceneDataObj->getSceneData();
			}

			if (sData) {

				uint32_t length = sceneDataObj->getDataSize() - sizeof(SceneSaveData);

				str = gcnew System::String(sData->name, 0, length - 1); // -1 cus C# is derpy and does not ignore \0

				data->SkyColor = *toColor(sData->skyColor[0], sData->skyColor[1], sData->skyColor[2], sData->skyColor[3]);
				data->HasFog = sData->hasFog ? true : false;
				data->FogColorNear = *toColor(sData->fog[0], sData->fog[1], sData->fog[2], sData->fog[3]);
				data->FogColorFar = *toColor(sData->fog[4], sData->fog[5], sData->fog[6], sData->fog[7]);
				data->HasWater = sData->hasWater ? true : false;
			}

			if (str == nullptr) {
				str = gcnew System::String("Empty");
			}

			data->Deleted = e->deleteFlag ? true : false;
			data->Name = str;
			data->EditorID = e->formID;

			queryArgs->ReturnList->Add(data);
		}
	}

	//
	//{
	//	query.objectList = new IObject*[query.nrObjects];
	//	Extensions::extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);
	//	for (uint32_t i = 0; i < query.nrObjects; i++) {
	//		IObject* obj = query.objectList[i];
	//		SceneStuff* sc = (SceneStuff*)obj->getData();
	//		System::String^ str = gcnew System::String(obj->getName());
	//		Editor::DataSources::Scene^ data = gcnew Editor::DataSources::Scene();
	//		data->Deleted = obj->isDeleted() ? true : false;
	//		if (str == "") {
	//			str = "Empty";
	//		}
	//		data->Name = str;
	//		data->EditorID = obj->getFormID();
	//		if (sc != nullptr) {
	//			data->FogColorNear = *toColor(sc->fog[0], sc->fog[1], sc->fog[2], sc->fog[3]);//System::Windows::Media::Color::FromArgb(sc->fog[3] * 255.0f, sc->skyColor[0] * 255.0f, sc->skyColor[1] * 255.0f, sc->skyColor[2] * 255.0f);
	//			data->FogColorFar = *toColor(sc->fog[4], sc->fog[5], sc->fog[6], sc->fog[7]);//System::Windows::Media::Color::FromArgb(sc->skyColor[7] * 255.0f, sc->skyColor[4] * 255.0f, sc->skyColor[5] * 255.0f, sc->skyColor[6] * 255.0f);
	//			data->HasFog = sc->hasFog ? true : false;
	//			data->SkyColor = *toColor(sc->skyColor[0], sc->skyColor[1], sc->skyColor[2], sc->skyColor[3]);//System::Windows::Media::Color::FromArgb(sc->skyColor[3] * 255.0f, sc->skyColor[0] * 255.0f, sc->skyColor[1] * 255.0f, sc->skyColor[2] * 255.0f);
	//			data->HasWater = sc->hasWater ? true : false;
	//		}
	//		queryArgs->ReturnList->Add(data);
	//	}
	//	delete[] query.objectList;
	//}

	void GetRenderLayerData(Editor::EventHandler::QueryDataArgs^ queryArgs) {
		uint32_t tag = 0;
		memcpy(&tag, RENDERLAYER_TAG, 4);
		std::vector<uint32_t> objects = assetMan->getObjectsWithTag(tag);

		uint32_t nrObjects = objects.size();

		for (size_t i = 0; i < nrObjects; i++) {

			Entry* e = assetMan->getEntry(objects[i]);

			IRenderLayerDataObject* rDataObj = nullptr;

			RenderLayerSaveData* rData = nullptr;
			if (e->data) {
				rDataObj = assetMan->getConverter()->asRenderLayer(e->data);
				rData = rDataObj->getRenderLayerData();

			}
			System::String^ str = nullptr;
			Editor::DataSources::RenderLayer^ data = gcnew Editor::DataSources::RenderLayer();
			if (rData) {

				uint32_t length = rDataObj->getDataSize() - sizeof(RenderLayerSaveData);

				str = gcnew System::String(rData->name, 0, length - 1); // -1 cus C# is derpy and does not ignore \0

				data->DepthBuffer = rData->depthBuffer ? true : false;
				data->StencilBuffer = rData->stencilBuffer ? true : false;
				data->NumColorAttachments = rData->nrColorBuffers;
				data->ResolutionHeight = rData->height;
				data->ResolutionWidth = rData->width;
				data->ResolutionType = rData->resolutionType;
				data->ShaderProgram = rData->shaderProgramRef;

			}

			if (str == nullptr) {
				str = gcnew System::String("Empty");
			}

			data->Deleted = e->deleteFlag ? true : false;
			data->Name = str;
			data->EditorID = e->formID;

			queryArgs->ReturnList->Add(data);
		}

	}
	
	void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs) {

		switch (queryArgs->ObjectType) {
			case Editor::EventHandler::ObjectTypes::SCENE:
				GetSceneData(queryArgs);
				break;
			case Editor::EventHandler::ObjectTypes::RENDERLAYER:
				GetRenderLayerData(queryArgs);
				break;
			default:
				break;
		}

	}


}