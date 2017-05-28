#include "Editor.hpp"

#include "CoreInclude.hpp"
#include <iostream>
#include "ExtensionMap.hpp"

namespace Editor_clr {
	
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
		Extensions::extensionMap[callbackIndex] = ext;
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
		Editor::EventHandler::EventManager::onSaveEvent += gcnew System::EventHandler<Editor::EventHandler::SaveEventArgs^>(&Extensions::OnSaveEvent);
		Editor::EventHandler::EventManager::onQueryDataEvent += gcnew System::EventHandler<Editor::EventHandler::QueryDataArgs^>(&Extensions::OnQueryEvent);
		Editor::EventHandler::EventManager::onAddObjectEvent += gcnew System::EventHandler<Editor::EventHandler::AddObjectArgs^>(&Extensions::OnAddEvent);
		Editor::EventHandler::EventManager::onEditFormEvent += gcnew System::EventHandler<Editor::EventHandler::FormArgs^>(&Extensions::OnEditEvent);
		Editor::EventHandler::EventManager::onDeleteFormEvent += gcnew System::EventHandler<Editor::EventHandler::FormArgs^>(&Extensions::OnDeleteEvent);
		Editor::EventHandler::EventManager::onGetFormIDEvent += gcnew System::EventHandler<Editor::EventHandler::GetFormIDArgs^>(&Extensions::OnGetFormIDEvent);

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
}

EDITOR_API IEditor * CreateEditor() {
	return new Editor_clr::Editor_wrp();
}
