#ifndef EDITORWINDOW_HPP
#define EDITORWINDOW_HPP

#include <RenderEngine\IWindow.hpp>
#include <map>

class EditorWindow : public IWindow {
public:
	// Inherited via IWindow
	virtual void getCursorPos(int & x, int & y) override;
	virtual void setWindowPos(int x, int y) override;
	virtual void setWindowSize(int x, int y) override;
	virtual bool isVisible() override;
	virtual void showWindow(bool visible) override;
	virtual void setWindowBorderless(bool borderless) override;
	virtual void setWindowedTrueFullscreen(bool trueFullscreen) override;
	virtual void pollMessages() override;
	virtual void lockCursor(bool cursorLock) override;
	virtual void setWindowResizeCallback(WindowResizeCallback_t callback) override;
	virtual void setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback) override;
	virtual void setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback) override;
	virtual void setWindowScrollCallback(WindowScrollCallback_t callback) override;
	virtual void setWindowKeyboardCallback(WindowKeyCallback_t callback) override;
	virtual void setWindowCharacterCallback(WindowCharacterCallback_t callback) override;
	virtual void setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback) override;
	virtual void setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback) override;
	virtual void setWindowFocusCallback(WindowFocus_t callback) override;
	virtual void setWindowMouseDeltaCallback(WindowMouseDeltaCallback_t callback) override;
	virtual void setVsync(bool vSync) override;
	virtual void swapBuffers() override;
	virtual void setTitle(const char * title) override;
	virtual void * getNativeWindowHandle() override;

	WindowResizeCallback_t* resizeCallback = 0;
	WindowMouseMoveCallback_t* mouseMoveCallback = 0;
	WindowMouseButtonCallback_t* mouseButtonCallback = 0;
	WindowScrollCallback_t* scrollCallback = 0;
	WindowKeyCallback_t* keyCallback = 0;
	WindowCharacterCallback_t* characterCallback = 0;

	WindowControllerAxisCallback_t* controllerAxisCallback = 0;
	WindowControllerButtonCallback_t* controllerButtonCallback = 0;

	WindowFocus_t* focusCallback = 0;

	WindowMouseDeltaCallback_t* mouseDeltaCallback = 0;
};

ref class EditorWindowWrapper sealed : public Editor::EventHandler::IEditWindow {
public:
	virtual ~EditorWindowWrapper();

	EditorWindowWrapper(EditorWindow* window);

	virtual void WindowResizeCallback(System::Int32 w, System::Int32 h);
	virtual void WindowMouseMoveCallback(System::Int32 x, System::Int32 y);
	virtual void WindowMouseButtonCallback(System::Int32 button, System::Boolean pressed, System::Int32 modkeys);
	virtual void WindowScrollCallback(System::Int32 scrollX, System::Int32 scrollY);
	virtual void WindowKeyboardCallback(System::Int32 key, System::Boolean pressed, System::Int32 modkeys);
	virtual void WindowCharacterCallback(System::Int32 codepoint);
	virtual void WindowFocusCallback(System::Boolean focus);

	void poll();

private:
	EditorWindow* editWindow;

	int width, height;
	int sizeX, sizeY;

	std::map<int, bool>* mousePress;
	std::map<int, bool>* keyPress;
};

#endif