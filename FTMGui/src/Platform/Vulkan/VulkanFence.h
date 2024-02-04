#pragma once

#include "VulkanDevice.h"

namespace FTMGui {
	
	class VulkanFence
	{
	public:
		VulkanFence(const Ref<VulkanDevice>& Device);
		~VulkanFence();

		inline const VkFence GetHandle() const { return m_Fence; }

		void Wait();

	private:
		VkFence m_Fence;
		Ref<VulkanDevice> m_VkDevice;
	};

}