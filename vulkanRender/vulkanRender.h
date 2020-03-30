#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#ifdef _WIN64
	#include <Windows.h>
	#include <vulkan/vulkan_win32.h>
#endif

#pragma once
 

class vulkanRender
{
	
	
public:

	

	bool windowClose = false;
	vulkanRender() {};
	VkInstance vkInstance;
	VkSurfaceKHR vkSurface;
	VkPhysicalDevice VkPhysicalDevice;
	VkQueue graphicsQueue;
	VkDevice logicalDevice;


	std::vector<const char*>instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME,VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	std::vector<const char*>instanceLayers = { "VK_LAYER_KHRONOS_validation","VK_LAYER_LUNARG_api_dump" };
	std::vector<const char*>deviceExtensions = {};

	void initInstance();
	void createSurface(HINSTANCE hInstance);
	void pickPhysicalDevice();
	void CreateLogicalDevice();
	void renderLoop();
	void VkCleanup();
	void vulkan();
};
