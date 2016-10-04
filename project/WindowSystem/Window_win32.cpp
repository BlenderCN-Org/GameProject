#include "Window.hpp"
#include "Win32_init.hpp"

#ifdef _WIN32

#include <string>

Window::Window()
{
	WNDCLASSEX win_class{};

	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Create window with the registered class:
	RECT wr = { 0, 0, LONG(512), LONG(512) };
	AdjustWindowRectEx(&wr, style, FALSE, ex_style);
	windowHandle = CreateWindowEx(0,
								  WindowClassName,					// class name
								  L"asdf",				// app name
								  style,							// window style
								  CW_USEDEFAULT, CW_USEDEFAULT,		// x/y coords
								  wr.right - wr.left,				// width
								  wr.bottom - wr.top,				// height
								  NULL,								// handle to parent
								  NULL,								// handle to menu
								  GetModuleHandle(nullptr),			// hInstance
								  NULL);							// no extra parameters
	if ( !windowHandle ) {
		// It didn't work, so try to give a useful error:
		//assert(1 && "Cannot create a window in which to draw!\n");
		exit(-1);
	}

	oglRenderContext = 0;
	deviceContext = 0;
}

Window::~Window()
{
	deleteContext();

	DestroyWindow(windowHandle);
}

void Window::showWindow(bool visible)
{
	if ( visible )
		ShowWindow(windowHandle, SW_SHOW);
	else
		ShowWindow(windowHandle, SW_HIDE);
}

void Window::updateWindow()
{
	MSG msg;
	if ( PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE) ) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Window::isWindowVisible() const
{
	if(IsWindowVisible(windowHandle) == 0)
		return false;
	return true;
}

void Window::setWindowSize(int x, int y)
{
	RECT wr = { 0, 0, LONG(x), LONG(y) };

	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	AdjustWindowRectEx(&wr, style, FALSE, ex_style);

	SetWindowPos(windowHandle, 0, 0, 0, wr.right - wr.left, wr.bottom - wr.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::setWindowTitle(const char * title)
{
	std::string text(title);
	std::wstring wstr(text.begin(), text.end());
	SetWindowText(windowHandle, (LPWSTR)wstr.c_str());
}

void Window::createContext()
{

	deviceContext = GetDC(windowHandle);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // 32-bit z-buffer  
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	int  iPixelFormat;

	// get the best available match of pixel format for the device context   
	iPixelFormat = ChoosePixelFormat(deviceContext, &pfd);

	// make that the pixel format of the device context  
	SetPixelFormat(deviceContext, iPixelFormat, &pfd);

	
	oglRenderContext = wglCreateContext(deviceContext);
}

void Window::deleteContext()
{
	if ( oglRenderContext && deviceContext )
	{
		wglMakeCurrent(deviceContext, 0);
		wglDeleteContext(oglRenderContext);
		ReleaseDC(windowHandle, deviceContext);
		deviceContext = 0;
		oglRenderContext = 0;
	}
}

void Window::oglMakeCurrent()
{
	if(oglRenderContext && deviceContext )
		wglMakeCurrent(deviceContext, oglRenderContext);
}

void Window::swapBuffers()
{
	if(deviceContext )
		SwapBuffers(deviceContext);
}

#endif