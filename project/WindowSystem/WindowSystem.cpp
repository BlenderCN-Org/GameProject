#include "WindowSystem.hpp"
#include "Window.hpp"

#include <vld.h>

WINDOWSYSTEM_API WSWindow * wsCreateWindow(int width, int height, const char * title, WSMonitor * monitor, WSWindow * share)
{
	WSWindow* wnd = new WSWindow();

	wnd->window.setWindowSize(width, height);
	wnd->window.setWindowTitle(title);

	wnd->window.showWindow(true);

	return wnd;
}

WINDOWSYSTEM_API void wsPollEvents(WSWindow * window)
{
	window->window.updateWindow();
}

WINDOWSYSTEM_API void wsDestroyWindow(WSWindow * window)
{
	delete window;
	window = nullptr;
}

WINDOWSYSTEM_API bool wsIsWindowVisible(WSWindow * window)
{
	return window->window.isWindowVisible();
}

WINDOWSYSTEM_API void wsCreateOGLContext(WSWindow * window)
{
	window->window.createContext();
}

WINDOWSYSTEM_API void wsMakeCurrent(WSWindow * window)
{
	window->window.oglMakeCurrent();
}

WINDOWSYSTEM_API void wsSwapBuffers(WSWindow * window)
{
	window->window.swapBuffers();
}
