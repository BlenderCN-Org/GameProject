#include "DisplaySettings.hpp"
#include <sstream>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW\glfw3native.h>


DisplaySettings::DisplaySettings() : window(nullptr)
{
}

void DisplaySettings::setWindow(GLFWwindow * wnd)
{
	window = wnd;
}

void DisplaySettings::apply()
{
	if ( window )
	{
		//glfwSetWindowSize(window, screenRes.width, screenRes.height);
		glfwSwapInterval((int)vsync);

		const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		int xPos = vidMode->width/2;
		int yPos = vidMode->height/2;

		switch ( fullscreenMode )
		{
			case FullscreenMode::FULLSCREEN:
			{
				glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), xPos, yPos, screenRes.width, screenRes.height, vidMode->refreshRate);
				glfwPollEvents();
				break;
			}
			case FullscreenMode::WINDOWED_FULLSCREEN:
			{
				xPos -= vidMode->width / 2;
				yPos -= vidMode->height / 2;
				glfwSetWindowMonitor(window, NULL, xPos, yPos, vidMode->width, vidMode->height, vidMode->refreshRate);
				glfwPollEvents();

				HWND hwnd = glfwGetWin32Window(window);
				LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

				if ( style & WS_VISIBLE )
					style = WS_VISIBLE;
				SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_POPUP);

				//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

				const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwSetWindowMonitor(window, NULL, xPos, yPos, vidMode->width, vidMode->height, vidMode->refreshRate);
				
				break;
			}
			case FullscreenMode::WINDOWED:
			{
				xPos -= screenRes.width / 2;
				yPos -= screenRes.height / 2;
				glfwSetWindowMonitor(window, NULL, xPos, yPos, screenRes.width, screenRes.height, vidMode->refreshRate);
				glfwPollEvents();
				glfwSetWindowMonitor(window, NULL, xPos, yPos, screenRes.width, screenRes.height, vidMode->refreshRate);

				HWND hwnd = glfwGetWin32Window(window);
				LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

				if ( style & WS_VISIBLE )
					style = WS_VISIBLE;
				SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
				break;
			}
			case FullscreenMode::WINDOWED_BORDERLESS:
			{
				//glfwGetWindowSize(window, &screenRes.width, &screenRes.height);

				xPos -= screenRes.width / 2;
				yPos -= screenRes.height / 2;
				glfwSetWindowMonitor(window, NULL, xPos, yPos, screenRes.width-1, screenRes.height, vidMode->refreshRate);
				glfwPollEvents();
				
				HWND hwnd = glfwGetWin32Window(window);
				LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

				if ( style & WS_VISIBLE )
					style = WS_VISIBLE;
				SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_POPUP);
				//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				
				glfwSetWindowMonitor(window, NULL, xPos, yPos, screenRes.width, screenRes.height, vidMode->refreshRate);
				
				
				break;
			}
			default:
				break;
		}
	}
	
}

void DisplaySettings::setResolution(int width, int height)
{
	screenRes.width = width;
	screenRes.height = height;
}

void DisplaySettings::setResolution(Resolution res)
{
	screenRes = res;
}

Resolution DisplaySettings::getResolution() const
{
	return screenRes;
}

void DisplaySettings::getResolution(int & width, int & height)
{
	width = screenRes.width;
	height = screenRes.height;
}

void DisplaySettings::setFullscreenMode(FullscreenMode fmode)
{
	fullscreenMode = fmode;
}

FullscreenMode DisplaySettings::getFullscreenMode() const
{
	return fullscreenMode;
}

void DisplaySettings::setVsyncMode(VSyncMode vsMode)
{
	vsync = vsMode;
}

VSyncMode DisplaySettings::getVsyncMode() const
{
	return vsync;
}

void DisplaySettings::setFramerateLock(FramerateLock flock)
{
	fpsLock = flock;
}

FramerateLock DisplaySettings::getFrameLock() const
{
	return fpsLock;
}

Section DisplaySettings::getSection() const
{
	Section s;
	s.addString("Resolution", screenRes.toString());
	s.addInteger("Fullscreen", (int)fullscreenMode);
	s.addInteger("Vsync", (int)vsync);
	s.addInteger("Frame limiter", (int)fpsLock);

	return s;
}

std::string Resolution::toString() const
{
	std::stringstream ss;
	ss << width << "x" << height;
	return ss.str();
}
