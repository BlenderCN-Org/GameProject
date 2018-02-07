#include "Window.hpp"

// BaseWindow

BaseWindow::BaseWindow()
	: dinput8dev(nullptr)
	, inputDevice(nullptr)
	, resizeCallback(nullptr)
	, mouseMoveCallback(nullptr)
	, mouseButtonCallback(nullptr)
	, scrollCallback(nullptr)
	, keyCallback(nullptr)
	, characterCallback(nullptr)
	, controllerAxisCallback(nullptr)
	, controllerButtonCallback(nullptr)
	, focusCallback(nullptr)
	, mouseDeltaCallback(nullptr)
	, cursorLock(false)
	, modkeys(0)
	, inputProcessedByControllers(false)
	, width(0)
	, height(0) {}

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

void BaseWindow::setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback) {
	controllerAxisCallback = callback;
}

void BaseWindow::setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback) {
	controllerButtonCallback = callback;
}

VKWindow::VKWindow()
	: queue(VK_NULL_HANDLE)
	, presentPool(VK_NULL_HANDLE)
	, presentBuffers(nullptr)
	, imageAvaible(VK_NULL_HANDLE)
	, renderingFinished(VK_NULL_HANDLE)
	, currentImage(0)
	, waitFence(VK_NULL_HANDLE)
	, debugReportCallback(VK_NULL_HANDLE)
	, recreateSwapchain(false)
	, renderPass(VK_NULL_HANDLE) {}