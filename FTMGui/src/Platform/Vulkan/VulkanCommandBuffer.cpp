#include "VulkanCommandBuffer.h"

#include "VulkanRenderPass.h"

namespace FTMGui {
	
	VulkanCommandBuffer::VulkanCommandBuffer(const Ref<VulkanDevice>& Device,
											 const VulkanPhysicalDevice& PhysicalDevice)
		: m_CommandBuffer(nullptr), m_CommandPool(MakeScope<VulkanCommandPool>(Device, PhysicalDevice)), m_VkDevice(Device)
	{
		VkCommandBufferAllocateInfo AllocateInfo{};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_CommandPool->GetHandle();
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		AllocateInfo.commandBufferCount = 1;

		vkCall(vkAllocateCommandBuffers, m_VkDevice->GetHandle(), &AllocateInfo, &m_CommandBuffer);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

	void VulkanCommandBuffer::ResetCommandBuffer()
	{
		vkResetCommandBuffer(m_CommandBuffer, 0);
	}

}