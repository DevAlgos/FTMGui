#include "VulkanCommandPool.h"

namespace FTMGui {

	VulkanCommandPool::VulkanCommandPool(const Ref<VulkanDevice>& Device, 
										 const VulkanPhysicalDevice& PhysicalDevice)
		: m_VkDevice(Device), m_CommandPool(nullptr)
	{
		VkCommandPoolCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CreateInfo.queueFamilyIndex = PhysicalDevice.GetProperties().QueueFamily.GraphicsFamily.value();
		
		vkCall(vkCreateCommandPool, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_CommandPool);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		if(m_CommandPool)
			vkDestroyCommandPool(m_VkDevice->GetHandle(), m_CommandPool, nullptr);
	}
}