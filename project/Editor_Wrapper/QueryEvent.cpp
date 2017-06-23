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

	void GetSceneData(ExtensionQueryDataEvent &query, Editor::EventHandler::QueryDataArgs^ queryArgs) {

		query.objectList = new IObject*[query.nrObjects];

		Extensions::extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);

		for (uint32_t i = 0; i < query.nrObjects; i++) {

			IObject* obj = query.objectList[i];

			SceneStuff* sc = (SceneStuff*)obj->getData();

			System::String^ str = gcnew System::String(obj->getName());
			Editor::DataSources::Scene^ data = gcnew Editor::DataSources::Scene();
			data->Deleted = obj->isDeleted() ? true : false;
			data->Name = str;
			data->EditorID = obj->getFormID();
			data->FogColorNear = *toColor(sc->fog[0], sc->fog[1], sc->fog[2], sc->fog[3]);//System::Windows::Media::Color::FromArgb(sc->fog[3] * 255.0f, sc->skyColor[0] * 255.0f, sc->skyColor[1] * 255.0f, sc->skyColor[2] * 255.0f);
			data->FogColorFar = *toColor(sc->fog[4], sc->fog[5], sc->fog[6], sc->fog[7]);//System::Windows::Media::Color::FromArgb(sc->skyColor[7] * 255.0f, sc->skyColor[4] * 255.0f, sc->skyColor[5] * 255.0f, sc->skyColor[6] * 255.0f);
			data->HasFog = sc->hasFog ? true : false;
			data->SkyColor = *toColor(sc->skyColor[0], sc->skyColor[1], sc->skyColor[2], sc->skyColor[3]);//System::Windows::Media::Color::FromArgb(sc->skyColor[3] * 255.0f, sc->skyColor[0] * 255.0f, sc->skyColor[1] * 255.0f, sc->skyColor[2] * 255.0f);
			data->HasWater = sc->hasWater ? true : false;

			queryArgs->ReturnList->Add(data);
		}

		delete[] query.objectList;
	}

	void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs) {

		std::cout << "QueryEvent" << (int)queryArgs->ObjectType << "\n";

		if (Extensions::extensionMap.count(GET_OBJECTS_CALLBACK) >= 1 && Extensions::extensionMap[GET_OBJECTS_CALLBACK]) {

			int type = (int)queryArgs->ObjectType;

			if (type != 0) {

				if (queryArgs->ObjectType == Editor::EventHandler::ObjectTypes::DIALOG) {
					Editor::DataSources::MenuItem^ data = gcnew Editor::DataSources::MenuItem();

					data->Name = "Test";
					data->Command = "Foobar";
					queryArgs->ReturnList->Add(data);

					return;
				}

				ExtensionQueryDataEvent query;

				query.objectType = type;
				query.nrObjects = 0;
				query.objectList = nullptr;

				Extensions::extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);

				switch (queryArgs->ObjectType) {
					case Editor::EventHandler::ObjectTypes::SCENE:
					{
						GetSceneData(query, queryArgs);
					}
					break;
					default:
						break;
				}


			}
		}
	}


}