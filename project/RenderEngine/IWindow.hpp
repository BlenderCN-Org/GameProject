#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#define ACTION_BUTTON_DOWN 1
#define ACTION_BUTTON_UP   0

#define MODKEY_SHIFT 0x0001
#define MODKEY_CTRL 0x0002
#define MODKEY_ALT 0x0004
#define MODKEY_SUPER 0x0008

#define GAMEPAD_AXIS_X       0x0001
#define GAMEPAD_AXIS_Y       0x0002
#define GAMEPAD_AXIS_L       0x0004
#define GAMEPAD_AXIS_R       0x0008
#define GAMEPAD_AXIS_RT      0x0010
#define GAMEPAD_AXIS_LT      0x0020

#define GAMEPAD_BTN_DP_UP    0x0001
#define GAMEPAD_BTN_DP_DOWN  0x0002
#define GAMEPAD_BTN_DP_LEFT  0x0004
#define GAMEPAD_BTN_DP_RIGHT 0x0008
#define GAMEPAD_BTN_A        0x0010
#define GAMEPAD_BTN_B        0x0020
#define GAMEPAD_BTN_X        0x0040
#define GAMEPAD_BTN_Y        0x0080
#define GAMEPAD_BTN_START    0x0100
#define GAMEPAD_BTN_BACK     0x0200
#define GAMEPAD_BTN_L_THUMB  0x0400
#define GAMEPAD_BTN_R_THUMB  0x0800
#define GAMEPAD_BTN_LT       0x1000
#define GAMEPAD_BTN_RT       0x2000

class IWindow;

typedef void WindowResizeCallback_t(IWindow* window, int width, int height);
typedef void WindowMouseMoveCallback_t(IWindow* window, int xPos, int yPos);
typedef void WindowMouseDeltaCallback_t(IWindow* window, float dx, float dy);
typedef void WindowMouseButtonCallback_t(IWindow* window, int button, int action, int mods);
typedef void WindowScrollCallback_t(IWindow* window, int scrollX, int scrollY);
typedef void WindowKeyCallback_t(IWindow* window, int key, int action, int mods);
typedef void WindowCharacterCallback_t(IWindow* window, unsigned int codepoint);
typedef void WindowControllerAxisCallback_t(IWindow* window, unsigned int axis, float axisValue);
typedef void WindowControllerButtonCallback_t(IWindow* window, unsigned int button, int action);
typedef void WindowFocus_t(IWindow* window, bool focus);

class IWindow {
public:

	virtual void getCursorPos(int &x, int &y) = 0;

	virtual void setWindowPos(int x, int y) = 0;
	virtual void setWindowSize(int x, int y) = 0;
	virtual void getWindowSize(int &x, int &y) = 0;

	virtual bool isVisible() = 0;

	virtual void showWindow(bool visible) = 0;
	virtual void setWindowBorderless(bool borderless) = 0;
	virtual void setWindowedTrueFullscreen(bool trueFullscreen) = 0;

	virtual void pollMessages() = 0;

	virtual void lockCursor(bool cursorLock) = 0;

	virtual void setWindowResizeCallback(WindowResizeCallback_t callback) = 0;
	virtual void setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback) = 0;
	virtual void setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback) = 0;
	virtual void setWindowScrollCallback(WindowScrollCallback_t callback) = 0;
	virtual void setWindowKeyboardCallback(WindowKeyCallback_t callback) = 0;
	virtual void setWindowCharacterCallback(WindowCharacterCallback_t callback) = 0;

	virtual void setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback) = 0;
	virtual void setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback) = 0;

	virtual void setWindowFocusCallback(WindowFocus_t callback) = 0;

	virtual void setWindowMouseDeltaCallback(WindowMouseDeltaCallback_t callback) = 0;

	virtual void setVsync(bool vSync) = 0;

	virtual void swapBuffers() = 0;

	virtual void setTitle(const char* title) = 0;

	virtual void* getNativeWindowHandle() = 0;
	
	virtual void setCursorVisibility(bool visible) = 0;
};

#endif