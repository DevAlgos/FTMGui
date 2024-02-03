#include "VulkanPhysicalDevice.h"

namespace FTMGui {

	VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface)
		: m_PhysicalDevice(nullptr)
	{
		uint32_t DeviceCount = 0;
		vkEnumeratePhysicalDevices(instance.GetHandle(), &DeviceCount, nullptr);

		FTMGUI_ASSERT(DeviceCount > 0, "No devices support vulkan");
		
		std::vector<VkPhysicalDevice> Devices(DeviceCount);
		vkEnumeratePhysicalDevices(instance.GetHandle(), &DeviceCount, Devices.data());

		for (auto Device : Devices)
		{
			DeviceProperties Properties = Query(Device, surface);

			if (IsDeviceSuitable(Device, Properties))
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

	bool VulkanPhysicalDevice::IsDeviceSuitable(const VkPhysicalDevice& device, const DeviceProperties& Properties)
	{
		return Properties.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			   Properties.Features.geometryShader  && 
			   Properties.QueueFamily.IsValid()    && 
			  !Properties.SurfaceFormats.empty()   &&
			  !Properties.PresentModes.empty()     &&
			   ContainsRequiredExtensions(device);
	}

	bool VulkanPhysicalDevice::ContainsRequiredExtensions(const VkPhysicalDevice& device)
	{
		uint32_t ExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, nullptr);

		std::vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &ExtensionCount, AvailableExtensions.data());

		std::set <std::string> DesiredExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		for (const auto& Extension : AvailableExtensions)
		{
			DesiredExtensions.erase(Extension.extensionName);
		}

		return DesiredExtensions.empty();
	}

	DeviceProperties VulkanPhysicalDevice::Query(VkPhysicalDevice device, const VulkanSurface& surface)
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

			VkBool32 PresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, k, surface.Get(), &PresentSupport);

			if (PresentSupport)
				Prop.QueueFamily.PresentFamily = k;

			k++;
		}

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.Get(), &Prop.SurfaceCapabilites);

		uint32_t SurfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.Get(), &SurfaceFormatCount, nullptr);

		Prop.SurfaceFormats.resize(SurfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.Get(), &SurfaceFormatCount, Prop.SurfaceFormats.data());

		uint32_t PresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.Get(), &PresentModeCount, nullptr);

		Prop.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.Get(), &PresentModeCount, Prop.PresentModes.data());


		return Prop;
	}
}