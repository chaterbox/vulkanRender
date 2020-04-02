#include "vulkanRender.h"
#include <iostream>

#ifdef _DEBUG
bool enableValidation = true;
#else
bool enableValidation = false;
#endif
//instance
void vulkanRender::initInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Jovk";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "jovk engine";
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
#ifdef _WIN64
	instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif	
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
	if (enableValidation)
	{
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());
		instanceInfo.ppEnabledLayerNames = instanceLayers.data();
	}
	if (!enableValidation)
	{
		instanceInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&instanceInfo, nullptr, &vkInstance) != VK_SUCCESS) {
		throw std::runtime_error("");
	}
}
//window and surface
void vulkanRender::glfwCreatewindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 800, "vulkan", nullptr, nullptr);
}

void vulkanRender::createSurface(HWND hwnd,HINSTANCE hinstance)
{
	VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.flags = 0;
	surfaceInfo.hwnd = hwnd;
	surfaceInfo.hinstance = hinstance;

	if (vkCreateWin32SurfaceKHR(vkInstance, &surfaceInfo, nullptr, &vkSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create surface");
	};
}

void vulkanRender::createGlfwSurface()
{
	if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create surface");
	}
}
//physical device
void vulkanRender::pickPhysicalDevice()
{
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, nullptr);
	
	if (physicalDeviceCount > 0)
	{
		vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, &vkPhysicalDevice);
	}
	if (physicalDeviceCount == 0)
	{
		throw std::runtime_error("failed to find physical device");
	}
}
//logical device
void vulkanRender::createLogicalDevice()
{
	float QueueProriety = 1.0f;

	VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
	graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphicsQueueCreateInfo.pQueuePriorities = &QueueProriety;
	graphicsQueueCreateInfo.queueFamilyIndex = 0;
	graphicsQueueCreateInfo.queueCount = 1;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(vkPhysicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(logicalDevice, 0, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, 0, 0, &presentQueue);
}
//swapchain
void vulkanRender::createSwapchain()
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, vkSurface, &surfaceCapabilites);
	
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurface, &formatCount, nullptr);
	vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurface, &formatCount, &surfaceFormats);

	vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, 0, vkSurface, &surfaceSupport);
	
	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	swapchainCreateInfo.surface = vkSurface;
	swapchainCreateInfo.imageExtent.height = surfaceCapabilites.currentExtent.height;
	swapchainCreateInfo.imageExtent.width = surfaceCapabilites.currentExtent.width;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainCreateInfo.minImageCount = surfaceCapabilites.minImageCount;
	swapchainCreateInfo.imageArrayLayers = surfaceCapabilites.maxImageArrayLayers;
	swapchainCreateInfo.imageFormat = surfaceFormats.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormats.colorSpace;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = 0;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &vkSwapchainKHR) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swapchain");
	}

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(logicalDevice, vkSwapchainKHR, &imageCount, nullptr);
	vkGetSwapchainImagesKHR(logicalDevice, vkSwapchainKHR, &imageCount, &swapChainImage);

	}

void vulkanRender::recreateSwapchain()
{

}

void vulkanRender::createImageView()
{
	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

	if (vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &vkImageview)) {
		throw std::runtime_error("failed to create imageView");
	}
}

//cleanup
void vulkanRender::vkGlfwCleanup()
{
	vkDestroySwapchainKHR(logicalDevice, vkSwapchainKHR, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
	vkDestroyInstance(vkInstance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void vulkanRender::VkWIN32Cleanup()
{
	vkDestroySwapchainKHR(logicalDevice, vkSwapchainKHR, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
	vkDestroyInstance(vkInstance, nullptr);
	windowClose = true;
}
//run
void vulkanRender::vulkanWIN32(HWND hwnd,HINSTANCE hinstance)
{
	initInstance();
	createSurface(hwnd,hinstance);
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapchain();
	//createImageView();
}

void vulkanRender::vulkanGlfw()
{
	glfwCreatewindow();
	initInstance();
	createGlfwSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapchain();
	//createImageView();
	GlfwRenderLoop();
	vkGlfwCleanup();
}
//render loop
void vulkanRender::GlfwRenderLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
	}
	vkDeviceWaitIdle(logicalDevice);
}

void vulkanRender::WIN32renderLoop()
{
	while (windowClose)
	{
		drawFrame();
	}
	vkDeviceWaitIdle(logicalDevice);
}
//draw frame
void vulkanRender::drawFrame()
{

}