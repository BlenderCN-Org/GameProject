#include "ExtensionMap.hpp"
#include <iostream>	

namespace Extensions
{

	void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs) {

		std::cout << "QueryEvent" << (int)queryArgs->ObjectType << "\n";

		if (Extensions::extensionMap.count(GET_OBJECTS_CALLBACK) >= 1 && Extensions::extensionMap[GET_OBJECTS_CALLBACK]) {

			int type = (int)queryArgs->ObjectType;

			if (type != 0) {

				if (queryArgs->ObjectType == Editor::EventHandler::ObjectTypes::DIALOG)
				{
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

				query.objectList = new IObject*[query.nrObjects];

				Extensions::extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);

				for (uint32_t i = 0; i < query.nrObjects; i++) {

					IObject* obj = query.objectList[i];

					System::String^ str = gcnew System::String(obj->getName());
					Editor::DataSources::BaseData^ data = gcnew Editor::DataSources::BaseData();
					data->Name = str;
					data->EditorID = obj->getFormID();
					queryArgs->ReturnList->Add(data);
				}

				delete[] query.objectList;
			}
		}
	}


}