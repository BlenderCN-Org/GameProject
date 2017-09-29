#include "Window.hpp"

#if SUPPORT_VULKAN_WINDOW

// vulkan extensions
std::vector<const char*> deviceExtensionNames = { "VK_KHR_swapchain" };

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

void VKWindow::vulkanInitialize() {
	VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo{};
	debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCreateInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	debugReportCreateInfo.pfnCallback = vulkanDebugCallback;

	instanceData.instance = createInstance();

	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instanceData.instance, "vkCreateDebugReportCallbackEXT");
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instanceData.instance, "vkDestroyDebugReportCallbackEXT");

	if (fvkCreateDebugReportCallbackEXT && fvkDestroyDebugReportCallbackEXT) {
		fvkCreateDebugReportCallbackEXT(instanceData.instance, &debugReportCreateInfo, nullptr, &debugReportCallback);
	}

	uint32_t count = 0;
	VkResult r = vkEnumeratePhysicalDevices(instanceData.instance, &count, nullptr);
	VkPhysicalDevice* gpus = new VkPhysicalDevice[count];
	r = vkEnumeratePhysicalDevices(instanceData.instance, &count, gpus);

	if (count == 1) {
		// if only one gpu is avaible select it
		instanceData.gpu = gpus[0];
	} else {
		// else look for the first avaible discrete gpu
		VkPhysicalDeviceProperties prop{};
		for (size_t i = 0; i < count; i++) {
			vkGetPhysicalDeviceProperties(gpus[i], &prop);
			if (prop.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				instanceData.gpu = gpus[i];
				break;
			}
		}
	}
	delete gpus;

	float prio[] = { 1.0f };
	count = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(instanceData.gpu, &count, nullptr);

	if (count >= 1) {
		VkQueueFamilyProperties* props = new VkQueueFamilyProperties[count];
		vkGetPhysicalDeviceQueueFamilyProperties(instanceData.gpu, &count, props);

		for (size_t i = 0; i < count; i++) {
			if ((props[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
				instanceData.queueFamilyIndex = (uint32_t)i;
				break;
			}
		}
		delete props;
	}

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = prio;
	queueCreateInfo.queueFamilyIndex = instanceData.queueFamilyIndex;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensionNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames.data();

	r = vkCreateDevice(instanceData.gpu, &deviceCreateInfo, nullptr, &instanceData.device);
}

void VKWindow::createVkSwapchain() {
	createSwapchain(instanceData, swapchainData);

	swapchainData.swapchainImageCount = 0;
	VkResult r = vkGetSwapchainImagesKHR(instanceData.device, swapchainData.swapchain, &swapchainData.swapchainImageCount, nullptr);
	swapchainData.swapchainImages = new VkImage[swapchainData.swapchainImageCount];
	r = vkGetSwapchainImagesKHR(instanceData.device, swapchainData.swapchain, &swapchainData.swapchainImageCount, swapchainData.swapchainImages);

	vkGetDeviceQueue(instanceData.device, instanceData.queueFamilyIndex, 0, &queue);

	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = instanceData.queueFamilyIndex;

	r = vkCreateCommandPool(instanceData.device, &commandPoolCreateInfo, nullptr, &presentPool);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = swapchainData.swapchainImageCount;
	commandBufferAllocateInfo.commandPool = presentPool;

	presentBuffers = new VkCommandBuffer[swapchainData.swapchainImageCount];

	r = vkAllocateCommandBuffers(instanceData.device, &commandBufferAllocateInfo, presentBuffers);

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

	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(instanceData.device, &semaphoreCreateInfo, nullptr, &imageAvaible);
	vkCreateSemaphore(instanceData.device, &semaphoreCreateInfo, nullptr, &renderingFinished);

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	vkCreateFence(instanceData.device, &fenceCreateInfo, nullptr, &waitFence);

	for (uint32_t i = 0; i < swapchainData.swapchainImageCount; ++i) {
		VkImageMemoryBarrier barrier_from_present_to_clear = {
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
			nullptr,                                    // const void                            *pNext
			VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          srcAccessMask
			VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          dstAccessMask
			VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                          oldLayout
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          newLayout
			instanceData.queueFamilyIndex,              // uint32_t                               srcQueueFamilyIndex
			instanceData.queueFamilyIndex,              // uint32_t                               dstQueueFamilyIndex
			swapchainData.swapchainImages[i],           // VkImage                                image
			subresourceRange                            // VkImageSubresourceRange                subresourceRange
		};

		VkImageMemoryBarrier barrier_from_clear_to_present = {
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                        sType
			nullptr,                                    // const void                            *pNext
			VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                          srcAccessMask
			VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                          dstAccessMask
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout                          oldLayout
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                          newLayout
			instanceData.queueFamilyIndex,              // uint32_t                               srcQueueFamilyIndex
			instanceData.queueFamilyIndex,              // uint32_t                               dstQueueFamilyIndex
			swapchainData.swapchainImages[i],           // VkImage                                image
			subresourceRange                            // VkImageSubresourceRange                subresourceRange
		};

		vkBeginCommandBuffer(presentBuffers[i], &commandBeginInfo);
		vkCmdPipelineBarrier(presentBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);

		vkCmdClearColorImage(presentBuffers[i], swapchainData.swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);

		vkCmdPipelineBarrier(presentBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);
		vkEndCommandBuffer(presentBuffers[i]);
	}
}

void VKWindow::vulkanCleanup() {
	vkDeviceWaitIdle(instanceData.device);

	vkDestroyFence(instanceData.device, waitFence, nullptr);

	vkDestroySemaphore(instanceData.device, renderingFinished, nullptr);
	vkDestroySemaphore(instanceData.device, imageAvaible, nullptr);

	vkFreeCommandBuffers(instanceData.device, presentPool, swapchainData.swapchainImageCount, presentBuffers);
	vkDestroyCommandPool(instanceData.device, presentPool, nullptr);

	delete[] presentBuffers;
	delete[] swapchainData.swapchainImages;

	vkDestroySwapchainKHR(instanceData.device, swapchainData.swapchain, nullptr);
	vkDestroySurfaceKHR(instanceData.instance, instanceData.surface, nullptr);
	vkDestroyDevice(instanceData.device, nullptr);

	fvkDestroyDebugReportCallbackEXT(instanceData.instance, debugReportCallback, nullptr);
	vkDestroyInstance(instanceData.instance, nullptr);
	instanceData.instance = VK_NULL_HANDLE;
}

#endif