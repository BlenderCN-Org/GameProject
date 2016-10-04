#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW\glfw3.h>

class Window
{
public:

	Window();
	~Window();

	void showWindow();
	void hideWindow();

	void makeCurrent();

	void swapInterval(int interval);

	int shouldClose() const;

	void swapBuffers();

	void setTitle(const char* title);

//private:

	GLFWwindow* wnd;

};

#endif