#include "VulkanInstance.h"

#include "Window/GLFWBase.h"

#include <vector>

namespace FTMGui {

	static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			FTMGUI_LOG_INFO(messageType, " ", pCallbackData->pMessageIdName, " ", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			FTMGUI_LOG_INFO(messageType, " ", pCallbackData->pMessageIdName, " ", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			FTMGUI_LOG_WARN(messageType, " ", pCallbackData->pMessageIdName, " ", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			FTMGUI_ASSERT(false, messageType, " ", pCallbackData->pMessageIdName, " ", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			FTMGUI_ASSERT(false, messageType, " ", pCallbackData->pMessageIdName, " ", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}

	VulkanInstance::VulkanInstance(const std::string_view& ApplicationName)
		: m_Instance(nullptr)
	{
		VkApplicationInfo AppInfo{};
		AppInfo.sType					= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName		= ApplicationName.data();
		AppInfo.applicationVersion		= VK_MAKE_API_VERSION(0, 1, 0, 0);
		AppInfo.pEngineName				= "No Engine";
		AppInfo.engineVersion			= VK_MAKE_API_VERSION(0, 1, 0, 0);
		AppInfo.apiVersion				= VK_API_VERSION_1_3;

		VkInstanceCreateInfo CreateInfo{};
		CreateInfo.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		CreateInfo.pApplicationInfo		= &AppInfo;

		std::vector<const char*> Ext = GetExtensions();

	#ifdef VK_MAC
		CreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	#endif

		CreateInfo.enabledExtensionCount = (uint32_t)Ext.size();
		CreateInfo.ppEnabledExtensionNames = Ext.data();

	#ifdef VK_VALIDATION

		uint32_t LayerCount = 0;
		vkCall(vkEnumerateInstanceLayerProperties, &LayerCount, nullptr);

		std::vector<VkLayerProperties> AvailableLayers(LayerCount);
		vkCall(vkEnumerateInstanceLayerProperties, &LayerCount, AvailableLayers.data());

		std::vector<const char*> LayerNames;

		for (size_t i = 0; i < 6; i++)
		{
			for (auto& Layer : AvailableLayers)
			{
				if (Layer.layerName == DesiredValidation[i])
				{
					LayerNames.push_back(Layer.layerName);
					FTMGUI_LOG_INFO("Layer: ", Layer.layerName);
				}
			}
		}



		CreateInfo.enabledLayerCount = (uint32_t)LayerNames.size();
		CreateInfo.ppEnabledLayerNames = LayerNames.data();

		VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo{};

		MessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		MessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		MessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		MessengerCreateInfo.pfnUserCallback = vkDebugCallback;
		MessengerCreateInfo.pUserData = nullptr;

		CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&MessengerCreateInfo;
	#else
		CreateInfo.enabledLayerCount = 0;
		CreateInfo.pNext = nullptr;
	#endif


		vkCall(vkCreateInstance, &CreateInfo, nullptr, &m_Instance);

		SetupDebugUtils();
	}

	VulkanInstance::~VulkanInstance()
	{
		DestroyDebugUtils();

		vkCall(vkDestroyInstance, m_Instance, nullptr);
	}

	std::vector<const char*> VulkanInstance::GetExtensions()
	{
		uint32_t GlfwExtCount = 0;
		const char** GlfwExtensions{};

		GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtCount);

		std::vector<const char*> Extensions(GlfwExtensions, GlfwExtensions + GlfwExtCount);

	#ifdef VK_VALIDATION
		Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	#endif

	#ifdef VK_MAC
		Extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	#endif

		return Extensions;
	}

	void VulkanInstance::SetupDebugUtils()
	{
		auto fun = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
		if (!fun)
		{
			FTMGUI_LOG_WARN("unable to setup debug utils");
			return;
		}

#ifdef VK_VALIDATION

		VkDebugUtilsMessengerCreateInfoEXT DebugMessengerCreateInfo{};

		DebugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		DebugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
												   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
												   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		DebugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
											   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
											   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		DebugMessengerCreateInfo.pfnUserCallback = vkDebugCallback;
		DebugMessengerCreateInfo.pUserData = nullptr;

		fun(m_Instance, &DebugMessengerCreateInfo, nullptr, &m_DebugMessenger);
#endif
	}

	void VulkanInstance::DestroyDebugUtils()
	{
#ifdef VK_VALIDATION
		auto fun = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
		if (!fun)
			return;


		fun(m_Instance, m_DebugMessenger, nullptr);
#endif
	}

}