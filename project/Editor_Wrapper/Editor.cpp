#include "Editor.hpp"

#include "CoreInclude.hpp"
#include <iostream>

namespace Editor {

	void Editor::initializeEditor() {
		printf("Editor initialize\n");

		MainWindowWrapper ^ window = gcnew MainWindowWrapper();
		
		mw = &window;
	}

	void Editor::releaseEditor() {
		printf("I have no clue\n");
		delete this;
	}
}

EDITOR_API IEditor * CreateEditor() {
	return new Editor::Editor();
}
