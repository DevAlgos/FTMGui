#include "VulkanSwapchain.h"

#include "Window/GLFWBase.h"

#include <algorithm>

namespace FTMGui {

	VulkanSwapchain::VulkanSwapchain(const Ref<VulkanDevice>& VkDevice, 
									 const VulkanSurface& surface,
									 const VulkanPhysicalDevice& device, 
									 GLFWwindow* window)
		: m_SwapChain(nullptr), m_VkDevice(VkDevice)
	{
		VkSurfaceFormatKHR SurfaceFormat = ChooseSurfaceFormat(device);
		VkPresentModeKHR PresentMode = ChoosePresentMode(device);
		VkExtent2D ImageExtent = ChooseSwapExtent(device, window);

		m_Extent = ImageExtent;
		m_Format = SurfaceFormat.format;


		VkSwapchainCreateInfoKHR CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		CreateInfo.surface = surface.Get();
		CreateInfo.minImageCount = device.GetProperties().SurfaceCapabilites.minImageCount + 1;
		CreateInfo.imageFormat = SurfaceFormat.format;
		CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
		CreateInfo.imageExtent = ImageExtent;
		CreateInfo.imageArrayLayers = 1;
		CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t QueueFamilyIndices[2] = { device.GetProperties().QueueFamily.GraphicsFamily.value(),
										   device.GetProperties().QueueFamily.PresentFamily.value()
		};

		if (QueueFamilyIndices[0] != QueueFamilyIndices[1])
		{
			CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			CreateInfo.queueFamilyIndexCount = 2;
			CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
		}
		else
		{
			CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			CreateInfo.queueFamilyIndexCount = 0; 
			CreateInfo.pQueueFamilyIndices = nullptr; 
		}

		CreateInfo.preTransform = device.GetProperties().SurfaceCapabilites.currentTransform;
		CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		CreateInfo.presentMode = PresentMode;
		CreateInfo.clipped = VK_TRUE;
		CreateInfo.oldSwapchain = nullptr;

		vkCall(vkCreateSwapchainKHR, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &m_SwapChain);

		uint32_t ImageCount = 0;
		vkGetSwapchainImagesKHR(m_VkDevice->GetHandle(), m_SwapChain, &ImageCount, nullptr);

		m_RenderImages.resize(ImageCount);
		vkGetSwapchainImagesKHR(m_VkDevice->GetHandle(), m_SwapChain, &ImageCount, m_RenderImages.data());
	
		for (auto& Image : m_RenderImages)
		{
			VkImageView ImageView{};
			
			VkImageViewCreateInfo ImageViewCreateInfo{};
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = Image;
			ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ImageViewCreateInfo.format = m_Format;
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			ImageViewCreateInfo.subresourceRange.levelCount = 1;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			ImageViewCreateInfo.subresourceRange.layerCount = 1;

			vkCall(vkCreateImageView, m_VkDevice->GetHandle(), &ImageViewCreateInfo, nullptr, &ImageView);

			m_ImageViews.push_back(ImageView);

		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		for (auto& ImageView : m_ImageViews)
			vkDestroyImageView(m_VkDevice->GetHandle(), ImageView, nullptr);

		vkDestroySwapchainKHR(m_VkDevice->GetHandle(), m_SwapChain, nullptr);
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const VulkanPhysicalDevice& device)
	{
		//TODO could introduce a ranking system
		for (const auto& Format : device.GetProperties().SurfaceFormats)
		{
			if (Format.format == VK_FORMAT_B8G8R8A8_SRGB && Format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
				return Format;
		}

		return device.GetProperties().SurfaceFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const VulkanPhysicalDevice& device)
	{
		for (const auto& PresentMode : device.GetProperties().PresentModes)
		{
			if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return PresentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VulkanPhysicalDevice& device, GLFWwindow* window)
	{
		const auto& Capabilities = device.GetProperties().SurfaceCapabilites;

		if (Capabilities.currentExtent.width != UINT32_MAX)
			return Capabilities.currentExtent;
		
		int width, height;
		glfwGetFramebufferSize(window, &width, &height); 

		VkExtent2D Extent = { (uint32_t)width, (uint32_t)height };
		Extent.width = std::clamp(Extent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
		Extent.height = std::clamp(Extent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);
		return Extent;
	}

}