#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class MyApplication
{
private:
	GLFWwindow* window_;
	VkInstance instance_;
	int width = 800;
	int height = 600;
	constexpr static char APP_NAME[] = "valcan Application";
	VkDebugUtilsMessengerEXT debugMessenger_;
	static bool checkValidationLayerSupport(const std::vector<const char*> &validationLayers);
	static VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo();
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);
	static void initializeDebugMessenger(VkInstance &instance, VkDebugUtilsMessengerEXT &debugMessenger);

	void initializeVulkan();
	void initializeWindow();
	static void finalizeDebugMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger);
	void finalizeVulkan();
	void finalizeWindow();
	void mainLoop();
	static std::vector<const char*> getRequiredExtensions();


	static void createInstance(VkInstance *dest);

public:
	MyApplication() :window_(nullptr) {};
	~MyApplication();

	void ran();

};

