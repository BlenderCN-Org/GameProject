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
	virtual void getCursorPos(int &x, int &y);

	virtual void setWindowPos(int x, int y);
	virtual void setWindowSize(int x, int y);

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
	IDirectInput8* dinput8dev = nullptr;
	IDirectInputDevice8* inputDevice = nullptr;
	XINPUT_STATE lastState;
	HWND getWindowHandle();

	WindowResizeCallback_t* resizeCallback = 0;
	WindowMouseMoveCallback_t* mouseMoveCallback = 0;
	WindowMouseButtonCallback_t* mouseButtonCallback = 0;
	WindowScrollCallback_t* scrollCallback = 0;
	WindowKeyCallback_t* keyCallback = 0;
	WindowCharacterCallback_t* characterCallback = 0;

	WindowControllerAxisCallback_t* controllerAxisCallback = 0;
	WindowControllerButtonCallback_t* controllerButtonCallback = 0;

	WindowFocus_t* focusCallback = 0;

	WindowMouseDeltaCallback_t* mouseDeltaCallback = 0;

	bool cursorLock = false;
	int modkeys = 0;
	bool inputProcessedByControllers = false;

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

	VkQueue queue = VK_NULL_HANDLE;

	VkCommandPool presentPool = VK_NULL_HANDLE;
	VkCommandBuffer* presentBuffers = nullptr;

	VkSemaphore imageAvaible = VK_NULL_HANDLE;
	VkSemaphore renderingFinished = VK_NULL_HANDLE;

	uint32_t currentImage = 0;

	VkFence waitFence = VK_NULL_HANDLE;

	VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;

	bool recreateSwapchain = false;

	VkRenderPass renderPass = VK_NULL_HANDLE;
};

#endif

#if defined(SUPPORT_D3D12_WINDOW) && defined(_WIN32)
class D3D12Window : IWindow {
public:

private:
};

#endif

#endif