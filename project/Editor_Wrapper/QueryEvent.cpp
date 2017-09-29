#include "ExtensionMap.hpp"
#include <iostream>

using namespace System::Runtime::InteropServices;

namespace Extensions {
	size_t safe_strlen(const char* str) {
		if (str == nullptr) {
			return 0U;
		} else {
			return strlen(str);
		}
	}

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

		size_t nrObjects = objects.size();

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
				size_t length = safe_strlen(sData->name);
				if (length) {
					str = gcnew System::String(sData->name, 0, (int)length);
				}

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

	void GetRenderLayerData(Editor::EventHandler::QueryDataArgs^ queryArgs) {
		uint32_t tag = 0;
		memcpy(&tag, RENDERLAYER_TAG, 4);
		std::vector<uint32_t> objects = assetMan->getObjectsWithTag(tag);

		size_t nrObjects = objects.size();

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
				size_t length = safe_strlen(rData->name);
				if (length) {
					str = gcnew System::String(rData->name, 0, (int)length);
				}

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

	void GetStaticObjectData(Editor::EventHandler::QueryDataArgs^ queryArgs) {
		uint32_t tag = 0;
		memcpy(&tag, STATICOBJECT_TAG, 4);
		std::vector<uint32_t> objects = assetMan->getObjectsWithTag(tag);

		size_t nrObjects = objects.size();

		for (size_t i = 0; i < nrObjects; i++) {
			Entry* e = assetMan->getEntry(objects[i]);

			IStaticObjectDataObject* dataObj = nullptr;

			StaticObjectSaveData* sdata = nullptr;
			if (e->data) {
				dataObj = assetMan->getConverter()->asStaticObjectData(e->data);
				sdata = dataObj->getStaticObjectData();
			}
			System::String^ str = nullptr;
			System::String^ str2 = nullptr;
			Editor::DataSources::StaticObject^ data = gcnew Editor::DataSources::StaticObject();
			if (sdata) {
				size_t length = safe_strlen(sdata->name);
				if (length) {
					str = gcnew System::String(sdata->name, 0, (int)length);
				}
				length = safe_strlen(sdata->meshFile);
				if (length) {
					str2 = gcnew System::String(sdata->meshFile, 0, (int)length);
				}
				if (str2 == nullptr) {
					str2 = "";
				}
				// copy rest of data
				data->UseCollision = sdata->useCollision;
				data->MeshFile = str2;
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
			case Editor::EventHandler::ObjectTypes::STATIC:
				GetStaticObjectData(queryArgs);
				break;
			case Editor::EventHandler::ObjectTypes::STARTUPSCENE:
			{
				uint32_t id = assetMan->getStartupSceneRef();
				if (id != 0) {
					Entry* e = assetMan->getEntry(id);

					ISceneDataObject* sceneDataObj = nullptr;
					SceneSaveData* sData = nullptr;

					Editor::DataSources::Scene^ data = gcnew Editor::DataSources::Scene();
					System::String^ str = nullptr;

					if (e->data) {
						sceneDataObj = assetMan->getConverter()->asSceneData(e->data);
						sData = sceneDataObj->getSceneData();
					}

					if (sData) {
						size_t length = safe_strlen(sData->name);
						if (length) {
							str = gcnew System::String(sData->name, 0, (int)length);
						}
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
			break;
			default:
				break;
		}
	}
}