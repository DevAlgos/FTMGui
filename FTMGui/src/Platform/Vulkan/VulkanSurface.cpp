#include "VulkanSurface.h"

namespace FTMGui {

	VulkanSurface::VulkanSurface(GLFWwindow* window, const Ref<VulkanInstance>& Instance)
		: m_WindowSurface(nullptr), m_VkInstance(Instance)
	{
		//uses vkCreateWin32SurfaceKHR or equivlient but for different platforms 
		vkCall(glfwCreateWindowSurface, m_VkInstance->GetHandle(), window, nullptr, &m_WindowSurface);
	}

	VulkanSurface::~VulkanSurface()
	{
		if(m_WindowSurface)
			vkDestroySurfaceKHR(m_VkInstance->GetHandle(), m_WindowSurface, nullptr);
	}

}
