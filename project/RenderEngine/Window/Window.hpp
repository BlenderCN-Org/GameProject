#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../BuildOptions.hpp"

#include "../IWindow.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <dinput.h>
#include <Xinput.h>
#endif

#if SUPPORT_VULKAN_WINDOW
#include <vulkan\vulkan.hpp>
#include "VulkanWindowHelper.hpp"
#endif

class BaseWindow : public IWindow {
public:

	BaseWindow();

	virtual void getCursorPos(int &x, int &y);

	virtual void setWindowPos(int x, int y);
	virtual void setWindowSize(int x, int y);
	virtual void getWindowSize(int &x, int &y);

	virtual bool isVisible();

	virtual void showWindow(bool visible);
	virtual void setWindowBorderless(bool borderless);
	virtual void setWindowedTrueFullscreen(bool trueFullscreen);

	virtual void pollMessages() = 0;

	virtual void lockCursor(bool cursorLock);

	virtual void setWindowResizeCallback(WindowResizeCallback_t callback);
	virtual void setWindowMouseMoveCallback(WindowMouseMoveCallback_t callback);
	virtual void setWindowMouseButtonCallback(WindowMouseButtonCallback_t callback);
	virtual void setWindowScrollCallback(WindowScrollCallback_t callback);
	virtual void setWindowKeyboardCallback(WindowKeyCallback_t callback);
	virtual void setWindowCharacterCallback(WindowCharacterCallback_t callback);

	virtual void setWindowControllerAxisCallback(WindowControllerAxisCallback_t callback);
	virtual void setWindowControllerButtonCallback(WindowControllerButtonCallback_t callback);

	virtual void setWindowFocusCallback(WindowFocus_t callback);

	virtual void setWindowMouseDeltaCallback(WindowMouseDeltaCallback_t callback);
	//virtual void setVsync(bool vSync);

	virtual void setTitle(const char* title);

	virtual void* getNativeWindowHandle();

	//private:
	IDirectInput8* dinput8dev;
	IDirectInputDevice8* inputDevice;
	XINPUT_STATE lastState;
	HWND getWindowHandle();

	WindowResizeCallback_t* resizeCallback;
	WindowMouseMoveCallback_t* mouseMoveCallback;
	WindowMouseButtonCallback_t* mouseButtonCallback;
	WindowScrollCallback_t* scrollCallback;
	WindowKeyCallback_t* keyCallback;
	WindowCharacterCallback_t* characterCallback;

	WindowControllerAxisCallback_t* controllerAxisCallback;
	WindowControllerButtonCallback_t* controllerButtonCallback;

	WindowFocus_t* focusCallback;

	WindowMouseDeltaCallback_t* mouseDeltaCallback;

	bool cursorLock;
	bool hasFocus;
	int modkeys;
	bool inputProcessedByControllers;

	int width;
	int height;

protected:
	HWND windowHandle;
	bool visible;
};

class GLWindow : public BaseWindow {
public:

	void init();
	void deinit();

	virtual void setVsync(bool vSync);

	virtual void pollMessages();
	void makeCurrent();

	virtual void swapBuffers();

private:

#ifdef _WIN32

	HGLRC openglRenderContext;
	HDC deviceContext;

#endif
};

#if SUPPORT_VULKAN_WINDOW

class VKWindow : public BaseWindow {
public:

	VKWindow();

	void init();
	void deinit();

	virtual void setVsync(bool vSync);
	virtual void pollMessages();

	virtual void setWindowSize(int x, int y);

	virtual void swapBuffers();

private:

	void vulkanInitialize();
	void createVkSwapchain();
	void vulkanCleanup();

	VulkanInstance instanceData;

	VulkanSwapchain swapchainData;

	VkQueue queue;

	VkCommandPool presentPool;
	VkCommandBuffer* presentBuffers;

	VkSemaphore imageAvaible;
	VkSemaphore renderingFinished;

	uint32_t currentImage;

	VkFence waitFence;

	VkDebugReportCallbackEXT debugReportCallback;

	bool recreateSwapchain;

	VkRenderPass renderPass;
};

#endif

#if defined(SUPPORT_D3D12_WINDOW) && defined(_WIN32)
class D3D12Window : IWindow {
public:

private:
};

#endif

#endif