#pragma once

#pragma region Headers and Libraries
// Precompiled header
#include <pch.h>
// Vulkan Header files and Libraries
#include <vulkan/vulkan.h>
// Default Libraries
#include <stdexcept>
#include <cstdlib>

// GLFW Header files and Libraries
#include <GLFW/glfw3.h>
// GLM files
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
// GLFW Defines
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE

#pragma endregion

const int WindowWidth = 1280;
const int WindowHeight = 720;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayer = false;
#else
const bool enableValidationLayer = true;
#endif

class MainApplication
{
public:
	void Run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanUp();
	}

private:
	// Instance of GLFW window.
	GLFWwindow* window;
	// Vulkan instance
	VkInstance VulkanInstance;

	// Functions to run a window
	void initWindow() {
		// Initialize GLFW.
		glfwInit();

		// Send information to GLFW with the way we want a window to be created.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Create a window with GLFW.
		window = glfwCreateWindow(WindowWidth, WindowHeight, "Game Window", nullptr, nullptr);
	}
	void initVulkan() {
		createInstance();
	}
	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}
	void cleanUp() {
		// Destroy the Vulkan instance
		vkDestroyInstance(VulkanInstance, nullptr);

		// Destroy the Window we created
		glfwDestroyWindow(window);

		// Terminate the GLFW instance completely including all memory.
		glfwTerminate();
	}

	// information about the Vulkan Application.
	void createInstance() {

		if (enableValidationLayer && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		// App Info
		VkApplicationInfo appinfo = {};
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pApplicationName = "Cold Galaxy Game";
		appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.pEngineName = "Cold Galaxy Engine";
		appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appinfo.apiVersion = VK_API_VERSION_1_0;
		// Create Info
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appinfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		//createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &VulkanInstance);
		

		if (vkCreateInstance(&createInfo, nullptr, &VulkanInstance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance of Vulkan.");
		}
		
		// More cool code

		if (vkCreateInstance(&createInfo, nullptr, &VulkanInstance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		/*
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> v_extensions(extensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, v_extensions.data());

		std::cout << "available extensions: " << std::endl;

		for (const auto extension : v_extensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
		}
		*/
	}

	std::vector<const char*> getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayer) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				// Return false if no layers can be found.
				return false;
			}

			// Return true if atleast 1 layer can be found.
			return true;
		}
	}
};