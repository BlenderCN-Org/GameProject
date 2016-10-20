#ifndef IWINDOW_HPP
#define IWINDOW_HPP

class IWindow;

typedef void WindowResizeCallback_t(IWindow* window, int width, int height);
typedef void WindowMouseMoveCallback_t(IWindow* window, int xPos, int yPos);
typedef void WindowMouseButtonCallback_t(IWindow* window, int button, int action, int mods);
typedef void WindowScrollCallback_t(IWindow* window, int scrollX, int scrollY);
typedef void WindowKeyCallback(IWindow* window, int key, int action, int mods);

class IWindow
{

public:

	virtual void setWindowPos(int x, int y) = 0;
	virtual void setWindowSize(int x, int y) = 0;

	virtual void showWindow(bool visible) = 0;
	virtual void setWindowBorderless(bool borderless) = 0;

	virtual void setWindowResizeCallback(WindowResizeCallback_t callback) = 0;
	virtual void setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback) = 0;
	virtual void setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback) = 0;
	virtual void setWindowScrollCallback(WindowScrollCallback_t callback) = 0;
	virtual void setWindowKeyboardCallback() = 0;


};

#endif