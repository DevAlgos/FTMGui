#include "VulkanCommandBuffer.h"

namespace FTMGui {
	
	VulkanCommandBuffer::VulkanCommandBuffer(const Ref<VulkanDevice>& Device,
											 const VulkanPhysicalDevice& PhysicalDevice)
		: m_CommandBuffer(nullptr), m_CommandPool(MakeScope<VulkanCommandPool>(Device, PhysicalDevice)), m_VkDevice(Device)
	{
		VkCommandBufferAllocateInfo AllocateInfo{};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_CommandPool->GetHandle();
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		AllocateInfo.commandBufferCount = 1;

		vkCall(vkAllocateCommandBuffers, m_VkDevice->GetHandle(), &AllocateInfo, &m_CommandBuffer);
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

	void VulkanCommandBuffer::ResetCommandBuffer()
	{
		vkResetCommandBuffer(m_CommandBuffer, 0);
	}

	void VulkanCommandBuffer::RecordCommandBuffer(const VulkanFramebuffer& Framebuffer, 
												  const VulkanSwapchain& Swapchain,
												  const VulkanPipeline& Pipeline,
												  const VulkanRenderPass& RenderPass, 
												  uint32_t ImageIndex)
	{
		VkCommandBufferBeginInfo BeginInfo{};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.flags = 0;
		BeginInfo.pInheritanceInfo = nullptr;

		vkCall(vkBeginCommandBuffer, m_CommandBuffer, &BeginInfo);

		VkRenderPassBeginInfo RenderPassBeginInfo{};
		RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBeginInfo.renderPass = RenderPass.GetHandle();
		RenderPassBeginInfo.framebuffer = Framebuffer.GetHandle();
		RenderPassBeginInfo.renderArea.offset = { 0,0 };
		RenderPassBeginInfo.renderArea.extent = Swapchain.GetImageExtent();
		RenderPassBeginInfo.clearValueCount = 1;

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		RenderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline.GetHandle());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)(Swapchain.GetImageExtent().width);
		viewport.height = (float)(Swapchain.GetImageExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Swapchain.GetImageExtent();
		vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

		vkCmdDraw(m_CommandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(m_CommandBuffer);

		vkCall(vkEndCommandBuffer, m_CommandBuffer);
	}

}