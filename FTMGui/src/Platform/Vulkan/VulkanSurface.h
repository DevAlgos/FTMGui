#pragma once

#include "Window/GLFWBase.h"
#include "VulkanInstance.h"

namespace FTMGui {

	class VulkanSurface
	{
	public:
		VulkanSurface(GLFWwindow* window, const Ref<VulkanInstance>& Instance);
		~VulkanSurface();

		inline const VkSurfaceKHR Get() const { return m_WindowSurface; }

	private:
		VkSurfaceKHR m_WindowSurface;
		Ref<VulkanInstance> m_VkInstance;
	};

}