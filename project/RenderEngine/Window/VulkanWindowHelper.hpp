#ifndef VULKANWINDOWHELPER_HPP
#define VULKANWINDOWHELPER_HPP

#include <vulkan\vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags,
												   VkDebugReportObjectTypeEXT objType,
												   uint64_t sourcObj,
												   size_t location,
												   int32_t msgCode,
												   const char* layerPrefix,
												   const char* msg,
												   void* userData);

struct VulkanInstance {
	VkInstance instance;
	VkPhysicalDevice gpu;
	uint32_t queueFamilyIndex;
	VkDevice device;
	VkSurfaceKHR surface;
	VkFormat surfaceFormat;
	VkColorSpaceKHR colorSpace;
};

struct VulkanSwapchain {
	VkSwapchainKHR swapchain;
	uint32_t surfaceWidth;
	uint32_t surfaceHeight;
	uint32_t swapchainImageCount;
	VkImage* swapchainImages;
	VkImageView* swapchainImageViews;
};

VkInstance createInstance();
VkSwapchainKHR createSwapchain(const VulkanInstance &instanceData, uint32_t surfaceWidth, uint32_t surfaceHeight, VkSwapchainKHR oldSwapchain);

void createSwapchain(const VulkanInstance &instanceData, VulkanSwapchain &swapchain);

#endif