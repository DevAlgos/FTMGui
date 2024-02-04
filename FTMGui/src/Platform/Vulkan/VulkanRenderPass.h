#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace FTMGui {

	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const Ref<VulkanDevice>& Device,
						 const VulkanSwapchain& Swapchain);

		~VulkanRenderPass();

		inline const VkRenderPass GetHandle() const { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;

		Ref<VulkanDevice> m_VkDevice;
	};

}