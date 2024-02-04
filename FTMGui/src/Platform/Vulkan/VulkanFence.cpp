#include "VulkanFence.h"

namespace FTMGui {
	
	VulkanFence::VulkanFence(const Ref<VulkanDevice>& Device)
		: m_Fence(nullptr), m_VkDevice(Device)
	{
		VkFenceCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		CreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		
		vkCall(vkCreateFence, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_Fence);
	}

	VulkanFence::~VulkanFence()
	{
		if (m_Fence)
			vkDestroyFence(m_VkDevice->GetHandle(), m_Fence, nullptr);
	}
	void VulkanFence::Wait()
	{
		vkWaitForFences(m_VkDevice->GetHandle(), 1, &m_Fence, VK_TRUE, UINT64_MAX);
		vkResetFences(m_VkDevice->GetHandle(), 1, &m_Fence);
	}
}