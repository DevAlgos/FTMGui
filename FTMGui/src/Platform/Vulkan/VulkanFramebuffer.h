#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"

namespace FTMGui {

	class VulkanRenderPass;
	class VulkanSwapchain;

	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(const Ref<VulkanDevice>& Device, 
						  const VulkanRenderPass& RenderPass, 
			              const VulkanSwapchain& swapchain,
						  uint32_t ImageViewIndex);

		~VulkanFramebuffer();

		inline const VkFramebuffer GetHandle() const { return m_Framebuffer; }

	private:
		VkFramebuffer m_Framebuffer;
		Ref<VulkanDevice> m_VkDevice;
	};

}