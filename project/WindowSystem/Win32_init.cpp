#include "WindowSystem.hpp"
#include "Win32_init.hpp"

#ifdef _WIN32

WINDOWSYSTEM_API bool wsInit()
{
	return registerWindowClass();
}

WINDOWSYSTEM_API bool wsTerminate()
{
	return unregisterWindowClass();
}

LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg ) {
		case WM_CLOSE:
			ShowWindow(hWnd, SW_HIDE);
			return 0;
		case WM_SIZE:
			// we get here if the window has changed size, we should rebuild most
			// of our window resources before rendering to this window again.
			// ( no need for this because our window sizing by hand is disabled )
			break;
		default:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool registerWindowClass()
{

	// Initialize the window class structure:
	WNDCLASSEX win_class = {0};

	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WindowsEventHandler;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = GetModuleHandle(nullptr); // hInstance
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = WindowClassName;
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if ( !RegisterClassEx(&win_class) ) {
		// It didn't work, so try to give a useful error:
		return false;
	}

	return true;
}

bool unregisterWindowClass()
{
	int ret = UnregisterClass(WindowClassName, GetModuleHandle(nullptr));
	return (ret == 0);
}

#endif