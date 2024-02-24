#include "VulkanCommandPool.h"

namespace FTMGui {

	VulkanCommandPool::VulkanCommandPool(const Ref<VulkanDevice>& Device, 
										 const VulkanPhysicalDevice& PhysicalDevice, 
										 CommandPoolFlags Flags)
		: m_VkDevice(Device), m_CommandPool(nullptr)
	{
		VkCommandPoolCreateFlags CmdPoolFlags = 0;

		if(Flags & CREATE_RESET_COMMAND_BUFFER_BIT)
			CmdPoolFlags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (Flags & CREATE_TRANSIENT_BIT)
			CmdPoolFlags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;


		VkCommandPoolCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CreateInfo.flags = CmdPoolFlags;
		CreateInfo.queueFamilyIndex = PhysicalDevice.GetProperties().QueueFamily.GraphicsFamily.value();
		
		vkCall(vkCreateCommandPool, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_CommandPool);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		if(m_CommandPool)
			vkDestroyCommandPool(m_VkDevice->GetHandle(), m_CommandPool, nullptr);
	}
}