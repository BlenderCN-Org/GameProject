#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../IWindow.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

class BaseWindow : public IWindow
{

public:
	virtual void setWindowPos(int x, int y);
	virtual void setWindowSize(int x, int y);;

	virtual void showWindow(bool visible);
	virtual void setWindowBorderless(bool borderless);
	virtual void setWindowedTrueFullscreen(bool trueFullscreen);

	virtual void setWindowResizeCallback(WindowResizeCallback_t callback);
	virtual void setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback);
	virtual void setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback);
	virtual void setWindowScrollCallback(WindowScrollCallback_t callback);
	virtual void setWindowKeyboardCallback(WindowKeyCallback_t callback);

	//virtual void setVsync(bool vSync);

//private:

	WindowResizeCallback_t* resizeCallback;
	WindowMouseMoveCallback_t* mouseMoveCallback;
	WindowMouseButtonCallback_t* mouseButtonCallback;
	WindowScrollCallback_t* scrollCallback;
	WindowKeyCallback_t* keyCallback;

protected:
	HWND windowHandle;
	bool visible;

};

class GLWindow : public BaseWindow
{
public:

	void init();
	void deinit();

	virtual void setVsync(bool vSync);

private:

#ifdef _WIN32

	HGLRC openglRenderContext;
	HDC deviceContext;

#endif

};

#ifdef SUPPORT_VULKAN_WINDOW
class VKWindow : IWindow
{

public:

private:

};

#endif

#if defined(SUPPORT_D3D12_WINDOW) && defined(_WIN32)
class D3D12Window : IWindow
{

public:

private:

};

#endif

#endif