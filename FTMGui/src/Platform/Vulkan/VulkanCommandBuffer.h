#pragma once

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

namespace FTMGui {

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const Ref<VulkanDevice>& Device,
							const VulkanPhysicalDevice& PhysicalDevice, 
							CommandPoolFlags Flags = 0);

		VulkanCommandBuffer(const VulkanCommandBuffer& Other) {};

		~VulkanCommandBuffer();

		void ResetCommandBuffer();

		inline const VkCommandBuffer GetHandle() const { return m_CommandBuffer; }

	private:
		VkCommandBuffer m_CommandBuffer;

		Scope<VulkanCommandPool> m_CommandPool;
		Ref<VulkanDevice> m_VkDevice;
	};
}