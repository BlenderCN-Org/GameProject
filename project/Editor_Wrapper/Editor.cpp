#include "Editor.hpp"

#include "CoreInclude.hpp"
#include <iostream>
#include <map>

namespace Editor_clr {

	static void OnSaveEvent(System::Object^ sender, Editor::EventHandler::SaveEventArgs^ saveArgs);
	static void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs);
	static void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs^ addArgs);

	std::map<int, IExtension<void>*> extensionMap;

	bool Editor_wrp::initializeEditor() {

		bool success = false;

		if (!initialized) {
			initialized = true;
			printf("Editor initialize\n");

			Application::EnableVisualStyles();

			wrapper = gcnew MainWindowWrapper();
			eventWrapper = gcnew EventWrapper();

			success = true;
		} else {
			printf("Editor already ininitalized\n");

		}
		return success;
	}

	void Editor_wrp::releaseEditor() {
		printf("I have no clue\n");
		delete this;
	}

	void Editor_wrp::registerExtension(int callbackIndex, IExtension<void> * ext) {
		extensionMap[callbackIndex] = ext;
	}

	void Editor_wrp::detach() {
		SendMessage((HWND)eventWrapper->windowPtr, WM_USER + 123, 0, 0);
	}

	void Editor_wrp::attach() {
		HWND editor = (HWND)wrapper->getGameWindowAreaHandle().ToPointer();
		HWND windowPtr = (HWND)eventWrapper->windowPtr;
		SetParent(windowPtr, editor);
		SetWindowPos(windowPtr, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	bool Editor_wrp::isRunning() {
		// it needs to be initialized to be runnning, so that is a good start :)
		bool running = initialized;

		if (wrapper.operator MainWindowWrapper ^ () == nullptr || wrapper->window == nullptr) {
			running = false;
		}

		return running;
	}

	void Editor_wrp::poll() {
		if (wrapper->window->WasResized()) {
			System::Windows::Size s = wrapper->window->getGameWindowSize();
			SendMessage((HWND)eventWrapper->windowPtr, WM_SIZE, SIZE_RESTORED, MAKELPARAM(s.Width, s.Height));
		}

	}

	void Editor_wrp::setGameWindow(void * windowPtr) {

		HWND editor = (HWND)wrapper->getGameWindowAreaHandle().ToPointer();

		eventWrapper->windowPtr = windowPtr;
		wrapper->window->Closing += gcnew System::ComponentModel::CancelEventHandler(eventWrapper, &EventWrapper::OnClosing);
		Editor::EventHandler::EventManager::onSaveEvent += gcnew System::EventHandler<Editor::EventHandler::SaveEventArgs^>(&OnSaveEvent);
		Editor::EventHandler::EventManager::onQueryDataEvent += gcnew System::EventHandler<Editor::EventHandler::QueryDataArgs^>(&OnQueryEvent);
		Editor::EventHandler::EventManager::onAddObjectEvent += gcnew System::EventHandler<Editor::EventHandler::AddObjectArgs^>(&OnAddEvent);

		SetParent((HWND)windowPtr, editor);
		SetWindowPos((HWND)windowPtr, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	}

	void EventWrapper::OnClosing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e) {

		System::String^ message = "Are you sure you want to close the editor";
		System::String^ capiton = "Close Editor?";

		Editor::CloseEditorMessageBox mb;

		System::Windows::Forms::DialogResult dlr = mb.ShowDialog();

		if (dlr == System::Windows::Forms::DialogResult::Yes) {
			SendMessage((HWND)windowPtr, WM_USER + 123, 0, 0);
		} else if (dlr == System::Windows::Forms::DialogResult::No) {
			ShowWindow((HWND)windowPtr, false);
		} else {
			e->Cancel = true;
		}

		//SetParent((HWND)windowPtr, NULL);
		//SetWindowPos((HWND)windowPtr, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//ShowWindow((HWND)windowPtr, true);
		//UpdateWindow((HWND)windowPtr);
	}

	static void OnSaveEvent(System::Object^ sender, Editor::EventHandler::SaveEventArgs^ saveArgs) {

		std::cout << "SaveEvent\n";
		Console::Write(": ");
		Console::WriteLine(saveArgs->PackData);

		ExtensionSaveEvent extSave;
		extSave.fileName = "blabla";
		extSave.pack = saveArgs->PackData;
		if (extensionMap.count(SAVE_CALLBACK) >= 1 && extensionMap[SAVE_CALLBACK]) {
			extensionMap[SAVE_CALLBACK]->execute(1, &extSave);
		}
	}

	static void OnQueryEvent(System::Object^ sender, Editor::EventHandler::QueryDataArgs^ queryArgs) {

		std::cout << "QueryEvent\n";
		
		if (extensionMap.count(GET_OBJECTS_CALLBACK) >= 1 && extensionMap[GET_OBJECTS_CALLBACK]) {
			
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

				extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);

				query.objectList = new char*[query.nrObjects];

				extensionMap[GET_OBJECTS_CALLBACK]->execute(1, &query);

				char** charArray = (char**)query.objectList;

				for (int i = 0; i < query.nrObjects; i++) {

					char* item = ((char**)query.objectList)[i];

					System::String^ str = gcnew System::String(item);
					Editor::DataSources::BaseData^ data = gcnew Editor::DataSources::BaseData();
					data->Name = str;
					data->EditorID = "1234";
					queryArgs->ReturnList->Add(data);
				}

				delete[] query.objectList;
			}
		}
	}

	static void OnAddEvent(System::Object^ sender, Editor::EventHandler::AddObjectArgs ^ addArgs)
	{
		std::cout << "AddEvent\n";

		if (extensionMap.count(ADD_OBJECT_CALLBACK) >= 1 && extensionMap[ADD_OBJECT_CALLBACK]) {

			int type =(int)addArgs->ObjectType;

			extensionMap[ADD_OBJECT_CALLBACK]->execute(0, nullptr);

		}
	}

}

EDITOR_API IEditor * CreateEditor() {
	return new Editor_clr::Editor_wrp();
}
