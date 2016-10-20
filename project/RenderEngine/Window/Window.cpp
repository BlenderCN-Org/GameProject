#include "Window.hpp"

// BaseWindow

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
