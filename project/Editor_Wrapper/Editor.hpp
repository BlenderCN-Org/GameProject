#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "IEditor.hpp"
#include <Windows.h>

#include "Window\MainWindowWrapper.hpp"

#include <vcclr.h>

namespace Editor_clr {

	ref class EventWrapper {
	public:
		void* windowPtr = nullptr;
		void OnClosing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e);

	};

	class Editor_wrp : public IEditor {

	public:

		virtual bool initializeEditor();
		virtual void releaseEditor();

		virtual void registerExtension(int callbackIndex, IExtension<void>* ext);

		virtual void detach();
		virtual void attach();

		virtual bool isRunning();

		virtual void poll();

		virtual void setGameWindow(void* windowPtr);

	private:

		bool initialized = false;

		gcroot<MainWindowWrapper^> wrapper;
		gcroot<EventWrapper^> eventWrapper;

	};

}
#endif
