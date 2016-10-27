#ifndef WINDOW_HPP
#define WINDOW_HPP

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


};

#endif