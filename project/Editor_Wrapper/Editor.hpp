#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <GameProject/Engine/Interfaces/IEditor.hpp>
#include "EditorWindow.hpp"

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

	class Editor_wrp : public Engine::Interfaces::IEditor {
	public:

		Editor_wrp() {
			editorStatus = Engine::Interfaces::EditorStatus::STOPPED;
		}

		virtual bool initializeEditor(IRenderEngine* re);
		virtual void releaseEditor();

		virtual void startEditor();
		virtual void stopEditor();

		//virtual void registerExtension(int callbackIndex, IExtension<void>* ext);

		virtual Engine::Interfaces::EditorStatus getStatus() const;

		virtual void update();

		virtual IWindow* getEditorWindow();

		virtual void postPixels(uint32_t width, uint32_t height);

		//virtual void setAssetManager(IAssetManager* assetMan);
		//virtual void setEditorAccess(IEditorAccess* editAccess);

	private:

		void poll();

		bool initialized = false;

		gcroot<MainWindowWrapper^> wrapper;
		gcroot<EventWrapper^> eventWrapper;
		gcroot<EditorWindowWrapper^> editorWindowWrapper;

		EditorWindow editWindow;

		Engine::Interfaces::EditorStatus editorStatus;

		IRenderEngine* renderEngine;

		IPixelBuffer* pixelBuffers[2];
		int pboIndex = 0;
		int pboNextIndex = 0;
	};
}
#endif