#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#ifdef _WIN64
	#include <Windows.h>
	#include <vulkan/vulkan_win32.h>
#endif

#pragma once
 

class vulkanRender
{
	//instance
	void initInstance();
	//window
	void glfwCreatewindow();
	void createSurface(HWND hwnd,HINSTANCE hinstance);
	void createGlfwSurface();
	//physical and logical device
	void pickPhysicalDevice();
	void createLogicalDevice();
	//swapchain
	void createSwapchain();
	void recreateSwapchain();
	//cleanup
	void vkGlfwCleanup();
	//draw frame
	void drawFrame();
public:
	vulkanRender() {};
	//instance
	VkInstance vkInstance;
	//window and surface
	bool windowClose = false;
	GLFWwindow* window;
	VkSurfaceKHR vkSurface;
	//physical device
	VkPhysicalDevice vkPhysicalDevice;
	//queues
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	//logical device
	VkDevice logicalDevice;
	//swapchain
	VkSwapchainKHR vkSwapchainKHR;
	VkSurfaceCapabilitiesKHR surfaceCapabilites;
	VkSurfaceFormatKHR surfaceFormats;
	VkBool32 surfaceSupport;
	std::vector<VkImage>swapChainImages;

	//extensions and layers
	std::vector<const char*>instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME,VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	std::vector<const char*>instanceLayers = { "VK_LAYER_KHRONOS_validation","VK_LAYER_LUNARG_api_dump"};
	std::vector<const char*>deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	//run
	void vulkanWIN32(HWND hwnd,HINSTANCE hinstance);
	void vulkanGlfw();
	//render loop
	void GlfwRenderLoop();
	void WIN32renderLoop();
	//cleanup
	void VkWIN32Cleanup();
};
