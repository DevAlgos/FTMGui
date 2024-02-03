#include "VulkanDevice.h"

#include <set>

namespace FTMGui {

	VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& PhysicalDevice)
		: m_Device(nullptr), m_PresentQueue(nullptr)
	
	{
		std::vector<VkDeviceQueueCreateInfo> DeviceQueueCreateInfos;
		std::set<uint32_t> UniqueFamilies = { PhysicalDevice.GetProperties().QueueFamily.GraphicsFamily.value(),
											  PhysicalDevice.GetProperties().QueueFamily.PresentFamily.value()};


		for (uint32_t Family : UniqueFamilies)
		{
			VkDeviceQueueCreateInfo  CreateInfo{};
			CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			CreateInfo.queueFamilyIndex = Family;
			CreateInfo.queueCount = 1;

			float QueuePriority = 1.0f;
			CreateInfo.pQueuePriorities = &QueuePriority;
			DeviceQueueCreateInfos.push_back(CreateInfo);
		}

		

		VkPhysicalDeviceFeatures deviceFeatures{}; //TODO come back to this

		VkDeviceCreateInfo DeviceCreateInfo{};
		DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceCreateInfo.pQueueCreateInfos = DeviceQueueCreateInfos.data();
		DeviceCreateInfo.queueCreateInfoCount = (uint32_t)DeviceQueueCreateInfos.size();
		DeviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		
		const char* ExtNames[1] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		DeviceCreateInfo.ppEnabledExtensionNames = ExtNames;
		DeviceCreateInfo.enabledExtensionCount = 1;

		/*note to me these are redunant in newer versions of vulkan but are simply placed here
		for backwards compatiblity if required, in newer version Device validation
		is the same as instance validation.*/

		uint32_t Count = 0;

#ifdef VK_VALIDATION

		vkEnumerateDeviceLayerProperties(PhysicalDevice.Get(), &Count, nullptr);

		std::vector<VkLayerProperties> LayerProperties(Count);
		vkEnumerateDeviceLayerProperties(PhysicalDevice.Get(), &Count, LayerProperties.data());

		std::vector<const char*> LayerNames;

		for (size_t i = 0; i < 6; i++)
		{
			for (auto& Layer : LayerProperties)
			{
				if (Layer.layerName == DesiredValidation[i])
					LayerNames.push_back(Layer.layerName);
			}
		}

		DeviceCreateInfo.enabledLayerCount = (uint32_t)(LayerNames.size());
		DeviceCreateInfo.ppEnabledLayerNames = LayerNames.data();

#endif

		vkCall(vkCreateDevice, PhysicalDevice.Get(), &DeviceCreateInfo, nullptr, &m_Device);

	
		vkGetDeviceQueue(m_Device, PhysicalDevice.GetProperties().QueueFamily.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, PhysicalDevice.GetProperties().QueueFamily.PresentFamily.value(), 0, &m_PresentQueue);
		
		
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_Device, nullptr);
	}
}