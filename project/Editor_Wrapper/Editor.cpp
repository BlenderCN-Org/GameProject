#include "Editor.hpp"

#include "CoreInclude.hpp"
#include <iostream>

namespace Editor_clr {
	

	void Editor_wrp::initializeEditor() {
		printf("Editor initialize\n");

		Application::EnableVisualStyles();

		wrapper = gcnew MainWindowWrapper();
		eventWrapper = gcnew EventWrapper();

	}

	void Editor_wrp::releaseEditor() {
		printf("I have no clue\n");
		delete this;
	}

	void Editor_wrp::poll() {}

	void Editor_wrp::setGameWindow(void * windowPtr) {
		
		HWND editor = (HWND)wrapper->getGameWindowAreaHandle().ToPointer();

		eventWrapper->windowPtr = windowPtr;
		wrapper->window->Closing += gcnew System::ComponentModel::CancelEventHandler(eventWrapper, &EventWrapper::OnClosing);

		SetParent((HWND)windowPtr, editor);
		SetWindowPos((HWND)windowPtr, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void EventWrapper::OnClosing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e) {
		
		System::String^ message = "Are you sure you want to close the editor";
		System::String^ capiton = "Close Editor?";

		Editor::CloseEditorMessageBox mb;

		System::Windows::Forms::DialogResult dlr = mb.ShowDialog();

		if ( dlr == System::Windows::Forms::DialogResult::Yes ) {
			SendMessage((HWND)windowPtr, WM_USER + 123, 0, 0);
		}
		else if ( dlr == System::Windows::Forms::DialogResult::No) {
			ShowWindow((HWND)windowPtr, false);
		}
		else {
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
