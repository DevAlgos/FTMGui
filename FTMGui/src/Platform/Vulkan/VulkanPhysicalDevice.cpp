#include "VulkanPhysicalDevice.h"

namespace FTMGui {

	VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanInstance& instance)
		: m_PhysicalDevice(nullptr)
	{
		uint32_t DeviceCount = 0;
		vkEnumeratePhysicalDevices(instance.GetHandle(), &DeviceCount, nullptr);

		FTMGUI_ASSERT(DeviceCount > 0, "No devices support vulkan");
		
		std::vector<VkPhysicalDevice> Devices(DeviceCount);
		vkEnumeratePhysicalDevices(instance.GetHandle(), &DeviceCount, Devices.data());

		for (auto Device : Devices)
		{
			DeviceProperties Properties = Query(Device);

			if (IsDeviceSuitable(Properties))
			{
				FTMGUI_LOG_INFO("Device ", Properties.Properties.deviceName, " chosen");
				m_PhysicalDevice = Device;
				m_DeviceProperties = Properties;
				break;
			}
		}

	}
	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{
	}

	//TODO define a set of minimum features a device must have
	bool VulkanPhysicalDevice::IsDeviceSuitable(const DeviceProperties& Properties)
	{
		return Properties.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			   Properties.Features.geometryShader && Properties.QueueFamily.IsValid();
	}

	DeviceProperties VulkanPhysicalDevice::Query(VkPhysicalDevice device)
	{
		DeviceProperties Prop{};

		vkGetPhysicalDeviceProperties(device, &Prop.Properties);
		vkGetPhysicalDeviceFeatures(device, &Prop.Features);

		uint32_t QueueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &QueueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> QueueFamilyProperties(QueueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &QueueFamilyCount, QueueFamilyProperties.data());

		uint32_t k = 0;
		for (const auto& Property : QueueFamilyProperties)
		{
			if (Property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				Prop.QueueFamily.GraphicsFamily = k;

			k++;
		}

		return Prop;
	}
}