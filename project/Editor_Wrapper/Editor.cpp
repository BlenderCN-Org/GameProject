#include "Editor.hpp"

#include "CoreInclude.hpp"
#include "ExtensionMap.hpp"
#include <iostream>


namespace Editor_clr {

	bool Editor_wrp::initializeEditor(IRenderEngine* re) {

		if (!initialized) {
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			editorStatus = EditorStatus::STOPPED;

			Editor::EventHandler::EventManager::onSaveEvent += gcnew System::EventHandler<Editor::EventHandler::SaveEventArgs^>(&Extensions::OnSaveEvent);
			Editor::EventHandler::EventManager::onQueryDataEvent += gcnew System::EventHandler<Editor::EventHandler::QueryDataArgs^>(&Extensions::OnQueryEvent);
			Editor::EventHandler::EventManager::onAddObjectEvent += gcnew System::EventHandler<Editor::EventHandler::AddObjectArgs^>(&Extensions::OnAddEvent);
			Editor::EventHandler::EventManager::onEditFormEvent += gcnew System::EventHandler<Editor::EventHandler::FormArgs^>(&Extensions::OnEditEvent);
			Editor::EventHandler::EventManager::onDeleteFormEvent += gcnew System::EventHandler<Editor::EventHandler::FormArgs^>(&Extensions::OnDeleteEvent);
			Editor::EventHandler::EventManager::onGetFormIDEvent += gcnew System::EventHandler<Editor::EventHandler::GetFormIDArgs^>(&Extensions::OnGetFormIDEvent);

			initialized = true;

			renderEngine = re;

			pixelBuffers[0] = renderEngine->createPixelBuffer();
			pixelBuffers[1] = renderEngine->createPixelBuffer();

			pixelBuffers[0]->init();
			pixelBuffers[1]->init();

			pixelBuffers[0]->resize(10, 10);
			pixelBuffers[1]->resize(10, 10);
			Extensions::assetMan = nullptr;
		}
		return true;
	}

	void Editor_wrp::releaseEditor() {
		if (initialized) {
			printf("Releasing editor\n");
			pixelBuffers[0]->release();
			pixelBuffers[1]->release();
			delete this;
			//GC::Collect();
			long long mem = GC::GetTotalMemory(true);
			printf("GC Mem allocated %I64d\n", mem);
		} else {
			delete this;
		}
	}

	void Editor_wrp::startEditor() {

		if (editorStatus == EditorStatus::STOPPED) {
			editorStatus = EditorStatus::STARTING;
			initialized = true;
			printf("Starting editor\n");

			Application::EnableVisualStyles();

			wrapper = gcnew MainWindowWrapper();
			eventWrapper = gcnew EventWrapper();

			wrapper->waitForInit();

			editorWindowWrapper = gcnew EditorWindowWrapper(&editWindow);
			wrapper->window->SetEditWindow(editorWindowWrapper);
			wrapper->window->onCloseEvent += gcnew System::EventHandler<Editor::EventHandler::CloseArgs^>(eventWrapper, &EventWrapper::OnClosing);

			editorStatus = EditorStatus::RUNNING;

		} else if (editorStatus == EditorStatus::RUNNING || editorStatus == EditorStatus::HIDDEN) {
			if (editorStatus == HIDDEN) {
				wrapper->window->invokeShow();
				editorStatus = RUNNING;
			}
			printf("Editor already started\n");
		} else {
			printf("Cannot start editor in current state");
		}
	}

	void Editor_wrp::stopEditor() {
		editorStatus = EditorStatus::STOPPING;
		if (editorWindowWrapper.operator EditorWindowWrapper ^ () != nullptr) {
			editorWindowWrapper->~EditorWindowWrapper();
			editorWindowWrapper = nullptr;
		}
		delete wrapper;
		delete eventWrapper;

		editorStatus = EditorStatus::STOPPED;
	}

	void Editor_wrp::registerExtension(int callbackIndex, IExtension<void> * ext) {
		Extensions::extensionMap[callbackIndex] = ext;
	}

	//bool Editor_wrp::isRunning() {
	//	// it needs to be initialized to be runnning, so that is a good start :)
	//	bool running = initialized;
	//
	//	if (wrapper.operator MainWindowWrapper ^ () == nullptr || wrapper->window == nullptr || eventWrapper->closeEditor == true) {
	//		running = false;
	//	}
	//
	//	return running;
	//}

	EditorStatus Editor_wrp::getStatus() const {
		return editorStatus;
	}

	void Editor_wrp::update() {

		if (eventWrapper->hideEditor) {
			GC::Collect();
			editorStatus = HIDDEN;
			eventWrapper->hideEditor = false;
		}

		if (eventWrapper->closeEditor) {
			stopEditor();
			eventWrapper->closeEditor = false;
		}

		//pixelBuffers[0]->resize(w, h);
		//pixelBuffers[1]->resize(w, h);

		switch (editorStatus) {
			case UNINITIALIZED:
				break;
			case STOPPED:
				break;
			case STARTING:
				break;
			case RUNNING:
			case HIDDEN:
				poll();
				break;
			case STOPPING:
				break;
			default:
				break;
		}

	}

	void Editor_wrp::poll() {
		if (editorWindowWrapper.operator EditorWindowWrapper ^ () != nullptr) {
			editorWindowWrapper->poll();
		}
	}

	IWindow * Editor_wrp::getEditorWindow() {
		return &editWindow;
	}

	void Editor_wrp::postPixels(uint32_t width, uint32_t height) {

		pboIndex = (pboIndex + 1) % 2;
		pboNextIndex = (pboIndex + 1) % 2;

		pixelBuffers[pboIndex]->resize(width, height);
		pixelBuffers[pboIndex]->read(width, height);

		void* data = pixelBuffers[pboNextIndex]->map();
		if (data) {
			uint32_t w, h;
			pixelBuffers[pboNextIndex]->getSize(w, h);

			System::IntPtr^ ptr = gcnew IntPtr(data);

			wrapper->window->DrawGameWindowPixels(*ptr, w, h);
		}
		pixelBuffers[pboNextIndex]->unMap();

	}

	void Editor_wrp::setAssetManager(IAssetManager* assetMan) {
		Extensions::assetMan = assetMan;
	}

	void EventWrapper::OnClosing(System::Object ^sender, Editor::EventHandler::CloseArgs ^e) {

		System::String^ message = "Are you sure you want to close the editor";
		System::String^ capiton = "Close Editor?";

		Editor::CloseEditorMessageBox mb;

		System::Windows::Forms::DialogResult dlr = mb.ShowDialog();

		if (dlr == System::Windows::Forms::DialogResult::Yes) {
			hideEditor = true;
			e->close = Editor::EventHandler::CloseType::HIDE;
		} else if (dlr == System::Windows::Forms::DialogResult::No) {
			e->close = Editor::EventHandler::CloseType::CLOSE;
			closeEditor = true;
		} else {
			e->close = Editor::EventHandler::CloseType::ABORT;
		}
	}
}

EDITOR_API IEditor * CreateEditor() {
	return new Editor_clr::Editor_wrp();
}
