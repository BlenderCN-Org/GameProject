#include "Editor.hpp"

namespace Editor {

	void Editor::initializeEditor() {}

	void Editor::releaseEditor() {}
}

EDITOR_API IEditor * CreateEditor() {
	return new Editor::Editor();
}
