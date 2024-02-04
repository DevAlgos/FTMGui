#include "VulkanSemaphore.h"

namespace FTMGui {

	VulkanSemaphore::VulkanSemaphore(const Ref<VulkanDevice>& Device)
		: m_Semaphore(nullptr), m_VkDevice(Device)
	{
		VkSemaphoreCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		vkCall(vkCreateSemaphore, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_Semaphore);
	}
	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(m_VkDevice->GetHandle(), m_Semaphore, nullptr);
	}
	void VulkanSemaphore::Wait(VkPipelineStageFlags* Flags, uint32_t nFlags)
	{
		VkSemaphoreWaitInfo WaitInfo{};
		WaitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
		WaitInfo.pSemaphores = &m_Semaphore;
		WaitInfo.semaphoreCount = 1;

		//TODO not ready come back
		vkWaitSemaphores(m_VkDevice->GetHandle(), &WaitInfo ,UINT64_MAX);
	}
}