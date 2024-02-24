#pragma once

#include <optional>

#include "VulkanInstance.h"
#include "VulkanSurface.h"

namespace FTMGui {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		inline constexpr bool IsValid() const { return GraphicsFamily.has_value() && PresentFamily.has_value(); }
	};

	struct DeviceProperties
	{
		VkPhysicalDeviceProperties Properties;
		VkPhysicalDeviceFeatures   Features;
		QueueFamilyIndices		   QueueFamily;

		VkSurfaceCapabilitiesKHR   SurfaceCapabilites;
		std::vector<VkSurfaceFormatKHR> SurfaceFormats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface);
		VulkanPhysicalDevice() = default;
		~VulkanPhysicalDevice();

		inline const VkPhysicalDevice  Get() const { return m_PhysicalDevice; }
		inline const DeviceProperties& GetProperties() const { return m_DeviceProperties; }

		void ReQuery(const VulkanSurface& surface);

	private:

		bool IsDeviceSuitable(const VkPhysicalDevice& device, const DeviceProperties& Properties);
		bool ContainsRequiredExtensions(const VkPhysicalDevice& device);

		DeviceProperties Query(VkPhysicalDevice device, const VulkanSurface& surface);

	private:
		VkPhysicalDevice m_PhysicalDevice;
		DeviceProperties m_DeviceProperties;
	};

}