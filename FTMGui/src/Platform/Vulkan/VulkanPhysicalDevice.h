#pragma once

#include "VulkanInstance.h"

#include <optional>

namespace FTMGui {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;

		inline constexpr bool IsValid() const { return GraphicsFamily.has_value(); }
	};

	struct DeviceProperties
	{
		VkPhysicalDeviceProperties Properties;
		VkPhysicalDeviceFeatures   Features;
		QueueFamilyIndices		   QueueFamily;
	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(const VulkanInstance& instance);
		~VulkanPhysicalDevice();

		inline const VkPhysicalDevice  Get() const { return m_PhysicalDevice; }
		inline const DeviceProperties& GetProperties() const { return m_DeviceProperties; }

	private:

		bool IsDeviceSuitable(const DeviceProperties& Properties);
		DeviceProperties Query(VkPhysicalDevice device);

	private:
		VkPhysicalDevice m_PhysicalDevice;
		DeviceProperties m_DeviceProperties;
	};

}