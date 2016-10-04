
#include "Platform.hpp"


class Window
{
public:
	Window();
	~Window();

	void showWindow(bool visible);
	void updateWindow();

	bool isWindowVisible() const;

	void setWindowSize(int x, int y);
	void setWindowTitle(const char* title);

	void createContext();
	void deleteContext();

	void oglMakeCurrent();
	void swapBuffers();

private:

	
#ifdef _WIN32

	
	HWND windowHandle;
	HDC deviceContext;
	HGLRC oglRenderContext;

#endif
};



struct WSWindow
{
	Window window;
};