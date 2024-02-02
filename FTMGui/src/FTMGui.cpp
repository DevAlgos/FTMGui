#include "FTMGui.h"
#include "Utils/Log.h"

namespace FTMGui {

	FTMGuiContext::FTMGuiContext(Platform platform, const WindowInfo& info)
		: m_MainWindow(info), m_Platform(platform)
	{
		Log::Init();
		m_VkInstance		= MakeScope<VulkanInstance>("FTMGui Application");
		m_VkPhysicalDevice	= MakeScope<VulkanPhysicalDevice>(GetScope(m_VkInstance));
	}

	FTMGuiContext::~FTMGuiContext()
	{
		glfwTerminate();
	}

	void FTMGuiContext::UpdateCtx()
	{
		m_MainWindow.Update();
	}

}