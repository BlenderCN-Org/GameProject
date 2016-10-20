#ifdef _WIN32

// local includes
#include "Window.hpp"

// global includes
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\GL.h>

#include <iostream>
#include <cstdint>

// globals
const char* windowClassName = "WindowClass";
bool windowClassInitialized = false;
bool openglInitialized = false;

// global pre-defines
HWND setupWindow();

// global function definitions
LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	BaseWindow* wnd = reinterpret_cast<BaseWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch ( msg ) {
		case WM_SIZE:
		{
			if ( wnd->resizeCallback )
				printf("callback stuff");
			printf("Resize\n");
		}
		break;
		default:
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void setPixelFormatOGL(HDC windowDC) {
	int selectedPixelFormat = 0;
	uint32_t foundPixelFormats = 0;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		8,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		24,                    // 32-bit z-buffer  
		8,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};

	if ( !openglInitialized ) {
		// todo create tempContext and init ogl extensions
		HWND window = setupWindow();
		HDC dc = GetDC(window);
		SetPixelFormat(dc, selectedPixelFormat, &pfd);
		HGLRC rc = wglCreateContext(dc);
		wglMakeCurrent(dc, rc);

		wglewInit();

		wglMakeCurrent(0, 0);
		wglDeleteContext(rc);
		ReleaseDC(window, dc);
		DestroyWindow(window);

		openglInitialized = true;
	}

	int intAttributes[] = { 0 };
	float floatAttributes[] = { 0 };

	if ( wglChoosePixelFormatARB ) {
		wglChoosePixelFormatARB(windowDC, intAttributes, floatAttributes, 1, &selectedPixelFormat, &foundPixelFormats);
	} else {
		selectedPixelFormat = ChoosePixelFormat(windowDC, &pfd);
	}


	SetPixelFormat(windowDC, selectedPixelFormat, &pfd);


}

int registerWindowClass() {
	if ( windowClassInitialized )
		return windowClassInitialized;
	WNDCLASSA wc{};

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = windowClassName;

	return RegisterClassA(&wc);
}

HWND setupWindow() {
	HWND windowHandle = NULL;
	windowClassInitialized = registerWindowClass() != 0 ? 1 : 0;
	if ( windowClassInitialized ) {
		windowHandle = CreateWindowExA(0,
									   windowClassName,
									   "Window",
									   WS_OVERLAPPEDWINDOW,
									   CW_USEDEFAULT, CW_USEDEFAULT,
									   CW_USEDEFAULT, CW_USEDEFAULT,
									   NULL,
									   NULL,
									   GetModuleHandle(NULL),
									   NULL);
	}

	return windowHandle;
}

HGLRC createOpenGLContext(HDC windowDC) {

	setPixelFormatOGL(windowDC);

	HGLRC renderingContext = 0;
	if ( wglCreateContextAttribsARB ) {
		int contextAttributes[] = { 0 };

		renderingContext = wglCreateContextAttribsARB(windowDC, NULL, contextAttributes);
	} else {
		renderingContext = wglCreateContext(windowDC);
	}

	return renderingContext;
}

// window classes 

// BaseWindow -> win32 implementation

void BaseWindow::setWindowPos(int x, int y) {
	SetWindowPos(windowHandle, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void BaseWindow::setWindowSize(int x, int y) {
	SetWindowPos(windowHandle, NULL, 0, 0, x, y, SWP_NOZORDER | SWP_NOMOVE);
}

void BaseWindow::showWindow(bool visible) {
	this->visible = visible;
	ShowWindow(windowHandle, visible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
}

void BaseWindow::setWindowBorderless(bool borderless) {

	DWORD style = (borderless ? WS_POPUP : WS_OVERLAPPEDWINDOW) | (visible ? WS_VISIBLE : 0);
	SetWindowLongPtr(windowHandle, GWL_STYLE, style);

}

void BaseWindow::setWindowedTrueFullscreen(bool trueFullscreen) {
	// todo figure out how to make exclusive fullscreen
}

// GLWindow -> win32 implementation

void GLWindow::init() {
	windowHandle = setupWindow();
	deviceContext = GetDC(windowHandle);
	openglRenderContext = createOpenGLContext(deviceContext);

	wglMakeCurrent(deviceContext, openglRenderContext);

	wglSwapIntervalEXT(1);
}

void GLWindow::deinit() {
	wglMakeCurrent(0, 0);
	wglDeleteContext(openglRenderContext);

	ReleaseDC(windowHandle, deviceContext);
	DestroyWindow(windowHandle);

}

void GLWindow::setVsync(bool vSync) {
	wglSwapIntervalEXT(vSync ? 1 : 0);
}

#endif