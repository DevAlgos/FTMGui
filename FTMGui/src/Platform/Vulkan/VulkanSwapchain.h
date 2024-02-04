#pragma once

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"

namespace FTMGui {

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(const Ref<VulkanDevice>& VkDevice,  
						const VulkanSurface& surface,
						const VulkanPhysicalDevice& device, 
						GLFWwindow* window);

		~VulkanSwapchain();


		inline const VkSwapchainKHR Get()					   const { return m_SwapChain; }
		inline const VkFormat GetImageFormat()				   const { return m_Format; }
		inline const VkExtent2D GetImageExtent()			   const { return m_Extent; }
		inline const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }

	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(const VulkanPhysicalDevice& device);
		VkPresentModeKHR   ChoosePresentMode(const VulkanPhysicalDevice& device);

		VkExtent2D		   ChooseSwapExtent(const VulkanPhysicalDevice& device, GLFWwindow* window);


	private:
		VkSwapchainKHR m_SwapChain;
		Ref<VulkanDevice> m_VkDevice;

		VkFormat   m_Format;
		VkExtent2D m_Extent;
		std::vector<VkImage> m_RenderImages;
		std::vector<VkImageView> m_ImageViews;
	};

}