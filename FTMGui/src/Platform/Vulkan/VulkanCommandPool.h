#pragma once

#include "VulkanDevice.h"

namespace FTMGui {

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(const Ref<VulkanDevice>& Device, const VulkanPhysicalDevice& PhysicalDevice);
		~VulkanCommandPool();

		inline const VkCommandPool GetHandle() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
		
		Ref<VulkanDevice> m_VkDevice;
	};

}