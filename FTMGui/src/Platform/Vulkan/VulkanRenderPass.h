#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"


namespace FTMGui {

	class VulkanCommandBuffer;
	class VulkanFramebuffer;
	class VulkanPipeline;

	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const Ref<VulkanDevice>& Device,
						 const VulkanSwapchain& Swapchain);

		~VulkanRenderPass();

		void BeginRenderPass(const VulkanCommandBuffer& Buffer,
							 const VulkanFramebuffer& Framebuffer,
							 const VulkanSwapchain& Swapchain,
							 const VulkanPipeline& Pipeline,
							 uint32_t ImageIndex) const;



		void EndRenderPass(const VulkanCommandBuffer& Buffer);

		inline const VkRenderPass GetHandle() const { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;

		Ref<VulkanDevice> m_VkDevice;
	};

}