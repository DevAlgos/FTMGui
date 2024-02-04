#pragma once

#include "VulkanCommandPool.h"
#include "VulkanSwapchain.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"

namespace FTMGui {
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const Ref<VulkanDevice>& Device,
							const VulkanPhysicalDevice& PhysicalDevice);

		~VulkanCommandBuffer();

		void ResetCommandBuffer();

		void RecordCommandBuffer(const VulkanFramebuffer& Framebuffer,
								 const VulkanSwapchain& Swapchain,
								 const VulkanPipeline& Pipeline,
								 const VulkanRenderPass& RenderPass, 
								 uint32_t ImageIndex);

		inline const VkCommandBuffer GetHandle() const { return m_CommandBuffer; }

	private:
		VkCommandBuffer m_CommandBuffer;

		Scope<VulkanCommandPool> m_CommandPool;
		Ref<VulkanDevice> m_VkDevice;
	};
}