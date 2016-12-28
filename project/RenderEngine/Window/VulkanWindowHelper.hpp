#ifndef VULKANWINDOWHELPER_HPP
#define VULKANWINDOWHELPER_HPP

#include <vulkan\vulkan.h>

struct VkDataStruct {
	VkDevice device;
	VkPhysicalDevice gpu;
	VkSurfaceKHR surface;
	VkFormat surfaceFormat;
	VkColorSpaceKHR colorSpace;
};

VkSwapchainKHR createSwapchain(VkDevice device, VkSurfaceKHR surface, VkFormat surfaceFormat, VkColorSpaceKHR colorSpace, uint32_t surfaceWidth, uint32_t surfaceHeight, VkSwapchainKHR oldSwapchain);

#endif