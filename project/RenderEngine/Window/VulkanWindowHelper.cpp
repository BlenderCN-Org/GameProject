#include "VulkanWindowHelper.hpp"

#include <sstream>
#include <iostream>
#include <vector>

std::vector<const char*> vulkanExtensionNames = { "VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
std::vector<const char*> vulkanInstanceNames = { "VK_LAYER_LUNARG_standard_validation" /*, "VK_LAYER_RENDERDOC_Capture" */ };

// vulkan debug callback

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugReportFlagsEXT flags,
												   VkDebugReportObjectTypeEXT objType,
												   uint64_t sourcObj,
												   size_t location,
												   int32_t msgCode,
												   const char* layerPrefix,
												   const char* msg,
												   void* userData) {
	std::ostringstream stream;

	if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT ) {
		stream << "INFO: ";
	} else if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT ) {
		stream << "WARNING: ";
	} else if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) {
		stream << "PERF: ";
	} else if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT ) {
		stream << "DEBUG: ";
	} else if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT ) {
		stream << "ERROR: ";
	}
	stream << "@[" << layerPrefix << "]: ";
	stream << msg << std::endl;

	if ( strcmp(layerPrefix, "MEM") != 0 ) {
		std::cout << stream.str();
	}

	if ( flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT ) {
		system("pause");
		//	MessageBoxA(NULL, stream.str().c_str(), "Vulkan Error!", MB_OK);
	}

	return VK_FALSE;
}

VkInstance createInstance() {
	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo createInfo{};

	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 37);
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo{};
	debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	debugReportCreateInfo.pfnCallback = vulkanDebugCallback;

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = (uint32_t)vulkanExtensionNames.size();
	createInfo.ppEnabledExtensionNames = vulkanExtensionNames.data();
	createInfo.enabledLayerCount = (uint32_t)vulkanInstanceNames.size();
	createInfo.ppEnabledLayerNames = vulkanInstanceNames.data();
	createInfo.pNext = &debugReportCreateInfo;

	VkInstance instance = VK_NULL_HANDLE;

	VkResult r = vkCreateInstance(&createInfo, nullptr, &instance);

	return instance;
}

VkSwapchainKHR createSwapchain(const VulkanInstance &instanceData, uint32_t surfaceWidth, uint32_t surfaceHeight, VkSwapchainKHR oldSwapchain) {
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;

	VkPresentModeKHR presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
	{
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(instanceData.gpu, instanceData.surface, &presentModeCount, nullptr);
		VkPresentModeKHR* supportedPresentModes = new VkPresentModeKHR[presentModeCount];
		vkGetPhysicalDeviceSurfacePresentModesKHR(instanceData.gpu, instanceData.surface, &presentModeCount, supportedPresentModes);

		for ( size_t i = 0; i < presentModeCount; i++ ) {
			if ( supportedPresentModes[i] == VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR ) {
				presentMode = supportedPresentModes[i];
				break;
			}
		}

		delete[] supportedPresentModes;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = instanceData.surface;
	swapchainCreateInfo.minImageCount = 2;
	swapchainCreateInfo.imageFormat = instanceData.surfaceFormat;
	swapchainCreateInfo.imageColorSpace = instanceData.colorSpace;
	swapchainCreateInfo.imageExtent.width = surfaceWidth;
	swapchainCreateInfo.imageExtent.height = surfaceHeight;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;

	VkResult r = vkCreateSwapchainKHR(instanceData.device, &swapchainCreateInfo, nullptr, &swapchain);

	if ( oldSwapchain != VK_NULL_HANDLE ) {
		vkDestroySwapchainKHR(instanceData.device, oldSwapchain, nullptr);
	}

	return swapchain;
}

void createSwapchain(const VulkanInstance &instanceData, VulkanSwapchain & swapchain) {
	VkPresentModeKHR presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
	{
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(instanceData.gpu, instanceData.surface, &presentModeCount, nullptr);
		VkPresentModeKHR* supportedPresentModes = new VkPresentModeKHR[presentModeCount];
		vkGetPhysicalDeviceSurfacePresentModesKHR(instanceData.gpu, instanceData.surface, &presentModeCount, supportedPresentModes);

		for ( size_t i = 0; i < presentModeCount; i++ ) {
			if ( supportedPresentModes[i] == VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR ) {
				presentMode = supportedPresentModes[i];
				break;
			}
		}

		delete[] supportedPresentModes;
	}

	VkSurfaceCapabilitiesKHR surfaceCapabilities{};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(instanceData.gpu, instanceData.surface, &surfaceCapabilities);

	if ( surfaceCapabilities.currentExtent.width < UINT32_MAX ) {
		swapchain.surfaceWidth = surfaceCapabilities.currentExtent.width;
		swapchain.surfaceHeight = surfaceCapabilities.currentExtent.height;
	}

	VkSwapchainKHR oldSwapchain = swapchain.swapchain;

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = instanceData.surface;
	swapchainCreateInfo.minImageCount = 2;
	swapchainCreateInfo.imageFormat = instanceData.surfaceFormat;
	swapchainCreateInfo.imageColorSpace = instanceData.colorSpace;
	swapchainCreateInfo.imageExtent.width = swapchain.surfaceWidth;
	swapchainCreateInfo.imageExtent.height = swapchain.surfaceHeight;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	swapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;

	VkResult r = vkCreateSwapchainKHR(instanceData.device, &swapchainCreateInfo, nullptr, &swapchain.swapchain);

	if ( oldSwapchain != VK_NULL_HANDLE ) {
		vkDestroySwapchainKHR(instanceData.device, oldSwapchain, nullptr);
	}
}