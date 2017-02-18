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

		virtual void initializeEditor();
		virtual void releaseEditor();

		virtual void poll();

		virtual void setGameWindow(void* windowPtr);

		void OnClosing(System::Object ^sender, System::ComponentModel::CancelEventArgs ^e);

	private:

		gcroot<MainWindowWrapper^> wrapper;
		gcroot<EventWrapper^> eventWrapper;

	};

}
#endif
