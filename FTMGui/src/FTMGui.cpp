#include "FTMGui.h"
#include "Utils/Log.h"

namespace FTMGui {

	FTMGuiContext::FTMGuiContext(Platform platform, const WindowInfo& info)
		: m_Platform(platform)
	{
		Log::Init();

		m_MainWindow = MakeScope<Window>(info);

		m_VkInstance = MakeRef<VulkanInstance>("FTMGui Application");

		m_MainSurface = MakeScope<VulkanSurface>(m_MainWindow->GetHandle(), m_VkInstance);

		m_VkPhysicalDevice	= MakeScope<VulkanPhysicalDevice>(GetRef(m_VkInstance), GetScope(m_MainSurface));
		m_VkDevice			= MakeScope<VulkanDevice>(GetScope(m_VkPhysicalDevice));

		
	}

	FTMGuiContext::~FTMGuiContext()
	{
		glfwTerminate();
	}

	void FTMGuiContext::UpdateCtx()
	{
		m_MainWindow->Update();
	}

}