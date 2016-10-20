#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../IWindow.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

class GLWindow : IWindow
{
public:

	void init();
	void deinit();

	virtual void setWindowPos(int x, int y);
	virtual void setWindowSize(int x, int y);;

	virtual void showWindow(bool visible);
	virtual void setWindowBorderless(bool borderless);

	virtual void setWindowResizeCallback();
	virtual void setWindowMouseMoveCallback();
	virtual void setWindowMouseButtonCallback();
	virtual void setWindowScrollCallback();
	virtual void setWindowKeyboardCallback();

private:

#ifdef _WIN32

	HWND windowHandle;
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