#include "vulkanRender.h"
#include <iostream>

#ifdef _DEBUG
bool enableValidation = true;
#else
bool enableValidation = false;
#endif

void vulkanRender::renderLoop()
{
	while (!windowClose)
	{
		
	}
}

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

void vulkanRender::createSurface(HINSTANCE hInstance)
{
	VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.flags = 0;
	surfaceInfo.hinstance = hInstance;

	if (vkCreateWin32SurfaceKHR(vkInstance, &surfaceInfo, nullptr, &vkSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create surface");
	};
}

void vulkanRender::pickPhysicalDevice()
{
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, nullptr);
	
	if (physicalDeviceCount > 0)
	{
		vkEnumeratePhysicalDevices(vkInstance, &physicalDeviceCount, &VkPhysicalDevice);
	}
	if (physicalDeviceCount == 0)
	{
		throw std::runtime_error("failed to find physical device");
	}
}

void vulkanRender::CreateLogicalDevice()
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

	if (vkCreateDevice(VkPhysicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device");
	}
}

void vulkanRender::VkCleanup()
{
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
	vkDestroyInstance(vkInstance, nullptr);
	windowClose = true;
}

void vulkanRender::vulkan()
{

}