#pragma once

#include "VulkanDevice.h"

namespace FTMGui {

	typedef uint32_t CommandPoolFlags;

	constexpr CommandPoolFlags CREATE_RESET_COMMAND_BUFFER_BIT = 0b00000000000000000000000000000001;
	constexpr CommandPoolFlags CREATE_TRANSIENT_BIT = 0b00000000000000000000000000000010;

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(const Ref<VulkanDevice>& Device, 
						  const VulkanPhysicalDevice& PhysicalDevice,
						  CommandPoolFlags Flags = 0);
		~VulkanCommandPool();

		inline const VkCommandPool GetHandle() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
		
		Ref<VulkanDevice> m_VkDevice;
	};

}