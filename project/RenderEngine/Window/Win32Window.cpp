#ifdef _WIN32

// local includes
#include "Window.hpp"
#include "WindowSystem.hpp"

// global includes
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\GL.h>

// std include
#include <iostream>
#include <cstdint>
#include <tchar.h>
#include <strsafe.h>

#define RI_MOUSE_HWHEEL 0x0800

// globals
const wchar_t* windowClassName = _T("WindowClass");
bool windowClassInitialized = false;
bool openglInitialized = false;

HHOOK g_hKeyboardHook;
BOOL g_bWindowActive;

// vulkan extensions
const char *vulkanExtensionNames[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };

// global pre-defines
HWND setupWindow();

// global function definitions

LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	BaseWindow* wnd = reinterpret_cast<BaseWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (msg) {
		break;
	case WM_SETFOCUS:
		if (wnd->focusCallback)
			wnd->focusCallback(wnd, true);
		return 1;
		break;
	case WM_KILLFOCUS:
		if (wnd->focusCallback)
			wnd->focusCallback(wnd, false);
		return 1;
		break;
	case WM_SIZE:
	{
		if (wnd && wnd->resizeCallback) {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			wnd->resizeCallback(wnd, width, height);
			printf("callback stuff");
		}
		printf("Resize\n");
	}
	break;
	case WM_MOVE:
	{
		int xPos = (int)(short)LOWORD(lParam);   // horizontal position 
		int yPos = (int)(short)HIWORD(lParam);   // vertical position
	}
	break;
	case WM_CLOSE:
		break;

	case WM_ACTIVATEAPP:
		// g_bWindowActive is used to control if the Windows key is filtered by the keyboard hook or not.
		if (wParam == TRUE)
			g_bWindowActive = true;
		else
			g_bWindowActive = false;
		break;

	case WM_CHAR:
	case WM_SYSCHAR:
	case WM_UNICHAR:
	{
		const bool plain = (msg != WM_SYSCHAR);

		if (msg == WM_UNICHAR && wParam == UNICODE_NOCHAR) {
			// WM_UNICHAR is not sent by Windows, but is sent by some
			// third-party input method engine
			// Returning TRUE here announces support for this message
			return TRUE;
		}
		unsigned int codepoint = (unsigned int)wParam;
		if (codepoint < 32 || (codepoint > 126 && codepoint < 160))
			return 0;

		if (wnd->characterCallback)
			wnd->characterCallback(wnd, codepoint);
		return 0;
	}
	break;
	case WM_INPUT:
	{
		if (wnd) {
			UINT dwSize;

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
				sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL) {
				return 0;
			}

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
				sizeof(RAWINPUTHEADER)) != dwSize) {
				//OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
				std::cerr << "GetRawInputData does not return correct size !\n";
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEKEYBOARD) {

				if (raw->data.keyboard.Flags == RI_KEY_MAKE) {
					if (raw->data.keyboard.VKey == VK_SHIFT) {
						wnd->modkeys |= MODKEY_SHIFT;
					} else if (raw->data.keyboard.VKey == VK_CONTROL) {
						wnd->modkeys |= MODKEY_CTRL;
					} else if (raw->data.keyboard.VKey == VK_MENU) {
						wnd->modkeys |= MODKEY_ALT;
					} else if (raw->data.keyboard.VKey == VK_LWIN || raw->data.keyboard.VKey == VK_RWIN) {
						wnd->modkeys |= MODKEY_SUPER;
					}
				} else {
					if (raw->data.keyboard.VKey == VK_SHIFT) {
						wnd->modkeys &= ~MODKEY_SHIFT;
					} else if (raw->data.keyboard.VKey == VK_CONTROL) {
						wnd->modkeys &= ~MODKEY_CTRL;
					} else if (raw->data.keyboard.VKey == VK_MENU) {
						wnd->modkeys &= ~MODKEY_ALT;
					} else if (raw->data.keyboard.VKey == VK_LWIN || raw->data.keyboard.VKey == VK_RWIN) {
						wnd->modkeys &= ~MODKEY_SUPER;
					}
				}

				if (wnd->keyCallback) {

					int action = raw->data.keyboard.Flags == RI_KEY_BREAK ? ACTION_BUTTON_UP : ACTION_BUTTON_DOWN;

					int mods = wnd->modkeys;

					wnd->keyCallback(wnd, raw->data.keyboard.MakeCode, action, mods);
					//printf("keyCallback\n");
				}

				//printf("key event\n");

			} else if (raw->header.dwType == RIM_TYPEMOUSE) {

				if (wnd->mouseButtonCallback) {

					int button = -1;
					int action = 0;
					int mods = wnd->modkeys;
					switch (raw->data.mouse.usButtonFlags) {
					case(RI_MOUSE_BUTTON_1_DOWN):
					{
						button = 0;
						action = ACTION_BUTTON_DOWN;
						break;
					}
					case(RI_MOUSE_BUTTON_1_UP):
					{
						button = 0;
						action = ACTION_BUTTON_UP;
						break;
					}
					case(RI_MOUSE_BUTTON_2_DOWN):
					{
						button = 1;
						action = ACTION_BUTTON_DOWN;
						break;
					}
					case(RI_MOUSE_BUTTON_2_UP):
					{
						button = 1;
						action = ACTION_BUTTON_UP;
						break;
					}
					case(RI_MOUSE_BUTTON_3_DOWN):
					{
						button = 2;
						action = ACTION_BUTTON_DOWN;
						break;
					}
					case(RI_MOUSE_BUTTON_3_UP):
					{
						button = 2;
						action = ACTION_BUTTON_UP;
						break;
					}
					case(RI_MOUSE_BUTTON_4_DOWN):
					{
						button = 3;
						action = ACTION_BUTTON_DOWN;
						break;
					}
					case(RI_MOUSE_BUTTON_4_UP):
					{
						button = 3;
						action = ACTION_BUTTON_UP;
						break;
					}
					case(RI_MOUSE_BUTTON_5_DOWN):
					{
						button = 4;
						action = ACTION_BUTTON_DOWN;
						break;
					}
					case(RI_MOUSE_BUTTON_5_UP):
					{
						button = 4;
						action = ACTION_BUTTON_UP;
						break;
					}
					default:
						break;
					}
					if (button != -1)
						wnd->mouseButtonCallback(wnd, button, action, mods);
					//printf("mouse button callback\n");

					//std::cout << button;
				}

				if (wnd->mouseMoveCallback) {

					POINT pt;
					GetCursorPos(&pt);

					ScreenToClient(hWnd, &pt);
					wnd->mouseMoveCallback(wnd, pt.x, pt.y);

					//printf("move callback\n");
				}

				if (wnd->mouseDeltaCallback) {
					float mouseDX = (float)raw->data.mouse.lLastX;
					float mouseDY = (float)raw->data.mouse.lLastY;

					wnd->mouseDeltaCallback(wnd, mouseDX, mouseDY);
					//printf("delta callback\n");
				}

				if (wnd->scrollCallback) {

					short scrollX = 0;
					short scrollY = 0;

					if (RI_MOUSE_WHEEL == raw->data.mouse.usButtonFlags) {
						scrollX = (short)raw->data.mouse.usButtonData;
					}
					if (RI_MOUSE_HWHEEL == raw->data.mouse.usButtonFlags) {
						scrollY = (short)raw->data.mouse.usButtonData;
					}

					wnd->scrollCallback(wnd, (int)scrollX, (int)scrollY);
				}

				if (wnd->cursorLock) {
					RECT rec;
					GetWindowRect(hWnd, &rec);

					int posX = (rec.right - rec.left) / 2;
					posX += rec.left;

					int posY = (rec.bottom - rec.top) / 2;
					posY += rec.top;

					SetCursorPos(posX, posY);
				}

				//printf("\nmouse event\n");

			}
			delete[] lpb;
			DefWindowProc(hWnd, msg, wParam, lParam);
			return 1;
		}
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

	if (!openglInitialized) {
		// todo create tempContext and init ogl extensions
		HWND window = setupWindow();
		HDC dc = GetDC(window);
		selectedPixelFormat = ChoosePixelFormat(dc, &pfd);
		SetPixelFormat(dc, selectedPixelFormat, &pfd);
		HGLRC rc = wglCreateContext(dc);
		wglMakeCurrent(dc, rc);

		//glewInit();
		wglewInit();

		wglMakeCurrent(0, 0);
		wglDeleteContext(rc);
		ReleaseDC(window, dc);
		DestroyWindow(window);

		openglInitialized = true;
	}
	// todo fix attribute stuff
	int intAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		//WGL_SWAP_METHOD_ARB, WGL_SWAP_UNDEFINED_ARB,
		0

	};
	float floatAttributes[] = { 0 };

	selectedPixelFormat = 0;

	if (wglChoosePixelFormatARB) {
		wglChoosePixelFormatARB(windowDC, intAttributes, floatAttributes, 1, &selectedPixelFormat, &foundPixelFormats);
	} else {
		selectedPixelFormat = ChoosePixelFormat(windowDC, &pfd);
	}


	SetPixelFormat(windowDC, selectedPixelFormat, &pfd);


}

int registerWindowClass() {
	if (windowClassInitialized)
		return windowClassInitialized;
	WNDCLASS wc{};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = windowClassName;

	return RegisterClass(&wc);
}

HWND setupWindow() {
	HWND windowHandle = NULL;
	if (windowClassInitialized) {
		windowHandle = CreateWindowEx(0,
			windowClassName,
			_T("Window"),
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
	if (wglCreateContextAttribsARB) {
		int contextAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};

		renderingContext = wglCreateContextAttribsARB(windowDC, NULL, contextAttributes);
	} else {
		renderingContext = wglCreateContext(windowDC);
	}

	return renderingContext;
}

// window classes 

// BaseWindow -> win32 implementation

void BaseWindow::getCursorPos(int & x, int & y) {

	POINT p;
	GetCursorPos(&p);

	ScreenToClient(windowHandle, &p);

	x = p.x;
	y = p.y;
}

void BaseWindow::setWindowPos(int x, int y) {
	SetWindowPos(windowHandle, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void BaseWindow::setWindowSize(int x, int y) {

	RECT r;
	GetClientRect(windowHandle, &r);

	r.right = x;
	r.bottom = y;

	AdjustWindowRectEx(&r, (DWORD)GetWindowLongPtr(windowHandle, GWL_STYLE), FALSE, (DWORD)GetWindowLongPtr(windowHandle, GWL_EXSTYLE));

	SetWindowPos(windowHandle, NULL, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOZORDER | SWP_NOMOVE);
}

bool BaseWindow::isVisible() {
	LONG_PTR style = GetWindowLongPtr(windowHandle, GWL_STYLE);

	if (style & WS_VISIBLE)
		return true;
	return false;
}

void BaseWindow::showWindow(bool visible) {
	this->visible = visible;
	ShowWindow(windowHandle, visible ? SW_SHOW : SW_HIDE);
}

void BaseWindow::setWindowBorderless(bool borderless) {

	DWORD style = (borderless ? WS_POPUP : WS_OVERLAPPEDWINDOW) | (visible ? WS_VISIBLE : 0);
	SetWindowLongPtr(windowHandle, GWL_STYLE, style);

}

void BaseWindow::setWindowedTrueFullscreen(bool trueFullscreen) {
	// todo figure out how to make exclusive fullscreen
}

void BaseWindow::setTitle(const char * title) {
	std::string t(title);
	std::wstring str(t.begin(), t.end());
	SetWindowText(windowHandle, str.c_str());
}

// GLWindow -> win32 implementation

void GLWindow::init() {
	windowHandle = setupWindow();

	RAWINPUTDEVICE rid[2];

	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = RIDEV_INPUTSINK;   // adds HID mouse and also ignores legacy mouse messages
	rid[0].hwndTarget = windowHandle;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = RIDEV_NOHOTKEYS;   // adds HID keyboard and also ignores legacy keyboard messages
	rid[1].hwndTarget = windowHandle;

	//g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

	if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE) {
		//registration failed. Call GetLastError for the cause of the error
		std::cerr << "register RID failed\n";
		std::cerr << GetLastError() << std::endl;
	}

	if (windowHandle) {
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

		deviceContext = GetDC(windowHandle);
		openglRenderContext = createOpenGLContext(deviceContext);

		if (!openglRenderContext)
			printf("something went wrong");
		if (wglMakeCurrent(deviceContext, openglRenderContext) == FALSE) {
			printf("making current failed\n");
		}

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(1);
	}

	//cursorLock = true;
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

void GLWindow::pollMessages() {
	MSG msg;
	while (PeekMessage(&msg, windowHandle, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GLWindow::makeCurrent() {
	if (wglMakeCurrent(deviceContext, openglRenderContext) == FALSE) {
		printf("making current failed\n");
	}
}

void GLWindow::swapBuffers() {
	SwapBuffers(deviceContext);
}

void VKWindow::init() {
	windowHandle = setupWindow();

	RAWINPUTDEVICE rid[2];

	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = RIDEV_INPUTSINK;   // adds HID mouse and also ignores legacy mouse messages
	rid[0].hwndTarget = windowHandle;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = RIDEV_NOHOTKEYS;   // adds HID keyboard and also ignores legacy keyboard messages
	rid[1].hwndTarget = windowHandle;

	//g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

	if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE) {
		//registration failed. Call GetLastError for the cause of the error
		std::cerr << "register RID failed\n";
		std::cerr << GetLastError() << std::endl;
	}

	if (windowHandle) {
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};

		appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 37);
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = 2;
		createInfo.ppEnabledExtensionNames = vulkanExtensionNames;


		VkResult r = vkCreateInstance(&createInfo, nullptr, &instance);

		uint32_t count = 0;
		r = vkEnumeratePhysicalDevices(instance, &count, nullptr);
		VkPhysicalDevice* gpus = new VkPhysicalDevice[count];
		r = vkEnumeratePhysicalDevices(instance, &count, gpus);

		if (count == 1) {
			// if only one gpu is avaible select it
			gpu = gpus[0];
		} else {
			// else look for the first avaible discrete gpu
			VkPhysicalDeviceProperties prop{};
			for (size_t i = 0; i < count; i++) {
				vkGetPhysicalDeviceProperties(gpus[i], &prop);
				if (prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					gpu = gpus[i];
					break;
				}
			}
		}
		delete gpus;

		float prio[] = { 1.0f };
		count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, nullptr);

		if (count >= 1) {
			VkQueueFamilyProperties* props = new VkQueueFamilyProperties[count];
			vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, props);

			for (size_t i = 0; i < count; i++) {
				if (props[i].queueFlags == VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
					familyIndex = (uint32_t)i;
					break;
				}
			}
			delete props;
		}

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = prio;
		queueCreateInfo.queueFamilyIndex = familyIndex;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;

		r = vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device);

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hwnd = windowHandle;
		surfaceCreateInfo.hinstance = NULL;

		r = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);

		VkExtent2D imgExtent{};
		imgExtent.width = 1280;
		imgExtent.height = 720;


		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = 2;
		swapchainCreateInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
		swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageExtent = imgExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;

		r = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);

		swapCount = 0;
		r = vkGetSwapchainImagesKHR(device, swapchain, &swapCount, nullptr);
		swapchainImages = new VkImage[swapCount];
		r = vkGetSwapchainImagesKHR(device, swapchain, &swapCount, swapchainImages);

		vkGetDeviceQueue(device, familyIndex, 0, &queue);

		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = familyIndex;

		r = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &presentPool);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = swapCount;
		commandBufferAllocateInfo.commandPool = presentPool;

		presentBuffers = new VkCommandBuffer[swapCount];

		r = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, presentBuffers);

		VkCommandBufferBeginInfo commandBeginInfo{};
		commandBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		VkClearColorValue clearColor = { 1.0f, 0.8f, 0.4f, 0.0f };

		VkImageSubresourceRange subresourceRange{};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;



		for (uint32_t i = 0; i < swapCount; ++i) {
			VkImageMemoryBarrier barrier_from_present_to_clear = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
				familyIndex,                                // uint32_t                               srcQueueFamilyIndex
				familyIndex,                                // uint32_t                               dstQueueFamilyIndex
				swapchainImages[i],                         // VkImage                                image
				subresourceRange                            // VkImageSubresourceRange                subresourceRange
			};

			VkImageMemoryBarrier barrier_from_clear_to_present = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
				nullptr,                                    // const void                            *pNext
				VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
				VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
				familyIndex,                                // uint32_t                               srcQueueFamilyIndex
				familyIndex,                                // uint32_t                               dstQueueFamilyIndex
				swapchainImages[i],                         // VkImage                                image
				subresourceRange                            // VkImageSubresourceRange                subresourceRange
			};

			vkBeginCommandBuffer(presentBuffers[i], &commandBeginInfo);
			vkCmdPipelineBarrier(presentBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);

			vkCmdClearColorImage(presentBuffers[i], swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);

			vkCmdPipelineBarrier(presentBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);
			vkEndCommandBuffer(presentBuffers[i]);
		}

	}

}

void VKWindow::deinit() {

	vkDeviceWaitIdle(device);

	vkFreeCommandBuffers(device, presentPool, swapCount, presentBuffers);
	vkDestroyCommandPool(device, presentPool, nullptr);

	delete presentBuffers;
	delete swapchainImages;

	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
	instance = VK_NULL_HANDLE;

	DestroyWindow(windowHandle);
}

void VKWindow::setVsync(bool vsync) {

}

void VKWindow::pollMessages() {
	MSG msg;
	while (PeekMessage(&msg, windowHandle, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void VKWindow::swapBuffers() {

	VkResult r = VkResult::VK_SUCCESS;
	uint32_t imageIndex = 0;
	do {
		r = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &imageIndex);
	} while (r != VkResult::VK_SUCCESS);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &presentBuffers[imageIndex];

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;

	r = vkQueuePresentKHR(queue, &presentInfo);
}

void initWindowSystem() {

	windowClassInitialized = registerWindowClass() != 0 ? 1 : 0;

	//RAWINPUTDEVICE rid[2];
	//
	//rid[0].usUsagePage = 0x01;
	//rid[0].usUsage = 0x02;
	//rid[0].dwFlags = 0;   // adds HID mouse and also ignores legacy mouse messages
	//rid[0].hwndTarget = 0;
	//
	//rid[1].usUsagePage = 0x01;
	//rid[1].usUsage = 0x06;
	//rid[1].dwFlags = RIDEV_NOHOTKEYS | RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	//rid[1].hwndTarget = 0;
	//
	////g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
	//
	//if ( RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE ) {
	//	//registration failed. Call GetLastError for the cause of the error
	//	std::cerr << "register RID failed\n";
	//	std::cerr << GetLastError() << std::endl;
	//}

}

void deinitWindowSystem() {
	openglInitialized = false;
	windowClassInitialized = false;
	UnregisterClass(windowClassName, GetModuleHandle(NULL));

	//UnhookWindowsHookEx(g_hKeyboardHook);
}

#endif
