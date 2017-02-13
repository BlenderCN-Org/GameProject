#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "IEditor.hpp"

#include "Window\MainWindow.hpp"

namespace Editor {

	class Editor : public IEditor {

	public:

		void initializeEditor();
		void releaseEditor();

	private:

		MainWindowWrapper^ *mw;
	};

}
#endif
