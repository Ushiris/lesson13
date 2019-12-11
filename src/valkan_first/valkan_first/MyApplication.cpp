#include "MyApplication.h"

void MyApplication::createInstance(VkInstance * dest)
{
	VkApplicationInfo appInfo = {  };
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = APP_NAME;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "My Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef _DEBUG
	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions) 
	{
		std::cout << "\t" << extension << std::endl;
	}
#endif

	if (enableValidationLayers) 
	{
		static const std::vector<const char*> validationLayers = 
		{
			"VK_LAYER_KHRONOS_validation"
		};

		// 検証レイヤーの確認
		if (!checkValidationLayerSupport(validationLayers)) 
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}

		// インスタンスへの設定
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = populateDebugMessengerCreateInfo();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)& debugCreateInfo;
	}

	if (vkCreateInstance(&createInfo, nullptr, dest) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create instance!");
	}
}

MyApplication::~MyApplication()
{
}

void MyApplication::ran()
{
	initializeWindow();
	initializeVulkan();

	mainLoop();

	finalizeVulkan();
	finalizeWindow();
}

bool MyApplication::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) 
	{
		if (![](const char* layerName, const auto &availableLayers) 
		{
			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) { return true; }
			}
			return false;
		}
			(layerName, availableLayers)) 
		{
			return false;
		}
	}

	return true;
}

VkDebugUtilsMessengerCreateInfoEXT MyApplication::populateDebugMessengerCreateInfo()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = [](
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) -> VKAPI_ATTR VkBool32
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	};

	return createInfo;
}

VkResult MyApplication::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugUtilsMessengerEXT * pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func == nullptr) return VK_ERROR_EXTENSION_NOT_PRESENT;
	return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void MyApplication::initializeDebugMessenger(VkInstance & instance, VkDebugUtilsMessengerEXT & debugMessenger)
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = populateDebugMessengerCreateInfo();// 生成情報の構築

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {// 生成
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void MyApplication::initializeVulkan()
{
	createInstance(&instance_);
	initializeDebugMessenger(instance_, debugMessenger_);
}

void MyApplication::initializeWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window_ = glfwCreateWindow(width, height, APP_NAME, nullptr, nullptr);
}

void MyApplication::finalizeDebugMessenger(VkInstance & instance, VkDebugUtilsMessengerEXT & debugMessenger)
{
	if (!enableValidationLayers) return;

	// vkCreateDebugUtilsMessengerEXTに対応しているか確認して実行
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func == nullptr) return;
	func(instance, debugMessenger, nullptr);
}

void MyApplication::finalizeVulkan()
{
	finalizeDebugMessenger(instance_, debugMessenger_);
	vkDestroyInstance(instance_, nullptr);
}

void MyApplication::finalizeWindow()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void MyApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window_))
	{
		glfwPollEvents();
	}
}

std::vector<const char*> MyApplication::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}
