#include "VulkanRenderPass.h"

namespace FTMGui {

	VulkanRenderPass::VulkanRenderPass(const Ref<VulkanDevice>& Device, 
									   const VulkanSwapchain& Swapchain)
		: m_RenderPass(nullptr), m_VkDevice(Device)
	{
		//TODO for now just a color buffer but we may want more and more customizable.

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Swapchain.GetImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		
		VkRenderPassCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		CreateInfo.attachmentCount = 1;
		CreateInfo.pSubpasses = &subpass;
		CreateInfo.subpassCount = 1;
		CreateInfo.pAttachments = &colorAttachment;
		CreateInfo.attachmentCount = 1;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;

		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		CreateInfo.dependencyCount = 1;
		CreateInfo.pDependencies = &dependency;

		vkCall(vkCreateRenderPass, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_RenderPass);
	}
	VulkanRenderPass::~VulkanRenderPass()
	{
		if(m_RenderPass)
			vkDestroyRenderPass(m_VkDevice->GetHandle(), m_RenderPass, nullptr);
	}
}