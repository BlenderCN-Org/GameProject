#include "EditorWindow.hpp"

void EditorWindow::getCursorPos(int & x, int & y) {}

void EditorWindow::setWindowPos(int x, int y) {}

void EditorWindow::setWindowSize(int x, int y) {}

bool EditorWindow::isVisible() {
	return true;
}

void EditorWindow::showWindow(bool visible) {}

void EditorWindow::setWindowBorderless(bool borderless) {}

void EditorWindow::setWindowedTrueFullscreen(bool trueFullscreen) {}

void EditorWindow::pollMessages() {}

void EditorWindow::lockCursor(bool cursorLock) {}

void EditorWindow::setWindowResizeCallback(WindowResizeCallback_t callback) {
	resizeCallback = callback;
}

void EditorWindow::setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback) {
	mouseMoveCallback = callback;
}

void EditorWindow::setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback) {
	mouseButtonCallback = callback;
}

void EditorWindow::setWindowScrollCallback(WindowScrollCallback_t callback) {
	scrollCallback = callback;
}

void EditorWindow::setWindowKeyboardCallback(WindowKeyCallback_t callback) {
	keyCallback = callback;
}

void EditorWindow::setWindowCharacterCallback(WindowCharacterCallback_t callback) {
	characterCallback = callback;
}

void EditorWindow::setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback) {
	controllerAxisCallback = callback;
}

void EditorWindow::setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback) {
	controllerButtonCallback = callback;
}

void EditorWindow::setWindowFocusCallback(WindowFocus_t callback) {
	focusCallback = callback;
}

void EditorWindow::setWindowMouseDeltaCallback(WindowMouseDeltaCallback_t callback) {
	mouseDeltaCallback = callback;
}

void EditorWindow::setVsync(bool vSync) {}

void EditorWindow::swapBuffers() {}

void EditorWindow::setTitle(const char * title) {}

void * EditorWindow::getNativeWindowHandle() {
	return nullptr;
}

EditorWindowWrapper::~EditorWindowWrapper() {
	delete mousePress;
	delete keyPress;
}

EditorWindowWrapper::EditorWindowWrapper(EditorWindow * window) : editWindow(window) {
	mousePress = new std::map<int, bool>();
	keyPress = new std::map<int, bool>();
}

void EditorWindowWrapper::WindowResizeCallback(System::Int32 w, System::Int32 h) {
	width = w;
	height = h;
	//if (editWindow != nullptr && editWindow->resizeCallback != nullptr) {
	//	editWindow->resizeCallback(editWindow, w, h);
	//}
}

void EditorWindowWrapper::WindowMouseMoveCallback(System::Int32 x, System::Int32 y) {
	sizeX = x;
	sizeY = y;
	//if (editWindow != nullptr && editWindow->mouseMoveCallback != nullptr) {
	//	editWindow->mouseMoveCallback(editWindow, x, y);
	//}
}

void EditorWindowWrapper::WindowMouseButtonCallback(System::Int32 button, System::Boolean pressed, System::Int32 modkeys) {
	bool pr = (pressed == true ? true : false);
	int btn = button;

	if (System::Threading::Monitor::TryEnter(this)) {
		(*mousePress)[btn] = pr;
		System::Threading::Monitor::Exit(this);
	}

	//if (editWindow != nullptr && editWindow->mouseButtonCallback != nullptr) {
	//	editWindow->mouseButtonCallback(editWindow, button, pressed, modkeys);
	//}
}

void EditorWindowWrapper::WindowScrollCallback(System::Int32 scrollX, System::Int32 scrollY) {
	if (editWindow != nullptr && editWindow->scrollCallback != nullptr) {
		editWindow->scrollCallback(editWindow, scrollX, scrollY);
	}
}

void EditorWindowWrapper::WindowKeyboardCallback(System::Int32 key, System::Boolean pressed, System::Int32 modkeys) {
	bool pr = (pressed == true ? true : false);
	int btn = key;

	if (System::Threading::Monitor::TryEnter(this)) {
		(*keyPress)[btn] = pr;
		System::Threading::Monitor::Exit(this);
	}
	//if (editWindow != nullptr && editWindow->keyCallback != nullptr) {
	//	editWindow->keyCallback(editWindow, key, pressed, modkeys);
	//}
}

void EditorWindowWrapper::WindowCharacterCallback(System::Int32 codepoint) {
	if (editWindow != nullptr && editWindow->characterCallback != nullptr) {
		editWindow->characterCallback(editWindow, codepoint);
	}
}

void EditorWindowWrapper::WindowFocusCallback(System::Boolean focus) {
	if (editWindow != nullptr && editWindow->focusCallback != nullptr) {
		editWindow->focusCallback(editWindow, focus);
	}
}

void EditorWindowWrapper::poll() {
	if (editWindow != nullptr) {
		if (editWindow->resizeCallback != nullptr) {
			editWindow->resizeCallback(editWindow, width, height);
		}

		if (editWindow->mouseMoveCallback != nullptr) {
			editWindow->mouseMoveCallback(editWindow, sizeX, sizeY);
		}

		System::Threading::Monitor::Enter(this);
		if (editWindow->mouseButtonCallback != nullptr) {
			std::map<int, bool>::const_iterator& it = (*mousePress).begin();
			for (it; it != (*mousePress).end(); it++) {
				editWindow->mouseButtonCallback(editWindow, it->first, it->second, 0);
			}
			(*mousePress).clear();
		}

		if (editWindow->keyCallback != nullptr) {
			std::map<int, bool>::const_iterator& it = (*keyPress).begin();
			for (it; it != (*keyPress).end(); it++) {
				editWindow->keyCallback(editWindow, it->first, it->second, 0);
			}
			(*keyPress).clear();
		}
		System::Threading::Monitor::Exit(this);
	}
}