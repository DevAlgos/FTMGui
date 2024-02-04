#pragma once

#include "VulkanDevice.h"

namespace FTMGui {
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(const Ref<VulkanDevice>& Device);
		~VulkanSemaphore();

		inline const VkSemaphore GetHandle() const { return m_Semaphore; }

		void Wait(VkPipelineStageFlags* Flags, uint32_t nFlags);

	private:
		VkSemaphore m_Semaphore;
		Ref<VulkanDevice> m_VkDevice;
	};
}