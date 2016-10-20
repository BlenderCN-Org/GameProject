#include "Window.hpp"

Window::Window()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	//glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	
	wnd = glfwCreateWindow(1280, 720, "Title", nullptr, nullptr);
}

Window::~Window()
{
	if ( wnd )
		glfwDestroyWindow(wnd);
}

void Window::showWindow()
{
	glfwShowWindow(wnd);
}

void Window::hideWindow()
{
	glfwHideWindow(wnd);
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(wnd);
}

void Window::swapInterval(int interval)
{
	glfwSwapInterval(interval);
}

int Window::shouldClose() const
{
	return glfwWindowShouldClose(wnd);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(wnd);
}

void Window::setTitle(const char * title)
{
	glfwSetWindowTitle(wnd, title);
}
