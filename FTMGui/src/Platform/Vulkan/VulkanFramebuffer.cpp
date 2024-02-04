#include "VulkanFramebuffer.h"

namespace FTMGui {

	VulkanFramebuffer::VulkanFramebuffer(const Ref<VulkanDevice>& Device, 
										 const VulkanRenderPass& RenderPass,
									     const VulkanSwapchain& swapchain, 
									     uint32_t ImageViewIndex)
		: m_Framebuffer(nullptr), m_VkDevice(Device)
	{
		VkFramebufferCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		CreateInfo.renderPass = RenderPass.GetHandle();
		CreateInfo.attachmentCount = 1;

		FTMGUI_ASSERT(ImageViewIndex < swapchain.GetImageViews().size(), 
					 "image view index out of range ",
					  ImageViewIndex);

		CreateInfo.pAttachments = &swapchain.GetImageViews()[ImageViewIndex];
		CreateInfo.width = swapchain.GetImageExtent().width;
		CreateInfo.height = swapchain.GetImageExtent().height;
		CreateInfo.layers = 1;

		vkCall(vkCreateFramebuffer, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_Framebuffer);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		if(m_Framebuffer)
			vkDestroyFramebuffer(m_VkDevice->GetHandle(), m_Framebuffer, nullptr);
	}
}