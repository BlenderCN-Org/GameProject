#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "IEditor.hpp"
#include "EditorWindow.hpp"
#include <Windows.h>

#include "Window\MainWindowWrapper.hpp"

#include <vcclr.h>

namespace Editor_clr {

	ref class EventWrapper {
	public:
		void* windowPtr = nullptr;
		void OnClosing(System::Object ^sender, Editor::EventHandler::CloseArgs ^e);

		bool closeEditor = false;
		bool hideEditor = false;

	};

	class Editor_wrp : public IEditor {

	public:

		Editor_wrp() {
			editorStatus = EditorStatus::STOPPED;
		}

		virtual bool initializeEditor();
		virtual void releaseEditor();

		virtual void startEditor();
		virtual void stopEditor();

		virtual void registerExtension(int callbackIndex, IExtension<void>* ext);

		virtual EditorStatus getStatus() const;

		virtual void update();

		virtual IWindow* getEditorWindow();

		virtual void postPixels(uint32_t width, uint32_t height, void* data);

	private:

		void poll();

		bool initialized = false;

		gcroot<MainWindowWrapper^> wrapper;
		gcroot<EventWrapper^> eventWrapper;
		gcroot<EditorWindowWrapper^> editorWindowWrapper;


		EditorWindow editWindow;

		EditorStatus editorStatus;

	};

}
#endif
