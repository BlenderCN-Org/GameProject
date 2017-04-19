#include "Window.hpp"

// BaseWindow

void BaseWindow::lockCursor(bool _cursorLock) {
	cursorLock = _cursorLock;
}

void BaseWindow::setWindowResizeCallback(WindowResizeCallback_t callback) {
	resizeCallback = callback;
}

void BaseWindow::setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback) {
	mouseMoveCallback = callback;
}

void BaseWindow::setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback) {
	mouseButtonCallback = callback;
}

void BaseWindow::setWindowScrollCallback(WindowScrollCallback_t callback) {
	scrollCallback = callback;
}

void BaseWindow::setWindowKeyboardCallback(WindowKeyCallback_t callback) {
	keyCallback = callback;
}

void BaseWindow::setWindowCharacterCallback(WindowCharacterCallback_t callback) {
	characterCallback = callback;
}

void BaseWindow::setWindowFocusCallback(WindowFocus_t callback) {
	focusCallback = callback;
}

void BaseWindow::setWindowMouseDeltaCallback(WindowMouseDeltaCallback_t callback) {
	mouseDeltaCallback = callback;
}

void BaseWindow::setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback)
{
	controllerAxisCallback = callback;
}

void BaseWindow::setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback)
{
	controllerButtonCallback = callback;
}
