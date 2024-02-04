#include "FTMGui.h"
#include "Utils/Log.h"

namespace FTMGui {

	const std::string TestVertexShader = R"(
    #version 460

    vec2 positions[3] = vec2[](
        vec2(0.0, -0.5),
        vec2(0.5, 0.5),
        vec2(-0.5, 0.5)
    );

    void main() 
    {
        gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    }
	)";

	const std::string TestFragmentShader = R"(

	#version 460

	layout(location = 0) out vec4 outColor;

	void main() 
	{
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	)";


	FTMGuiContext::FTMGuiContext(Platform platform, const WindowInfo& info)
		: m_Platform(platform)
	{
		Log::Init();

		m_MainWindow = MakeScope<Window>(info);

		m_VkInstance = MakeRef<VulkanInstance>("FTMGui Application");

		m_MainSurface = MakeScope<VulkanSurface>(m_MainWindow->GetHandle(), m_VkInstance);

		m_VkPhysicalDevice	= MakeScope<VulkanPhysicalDevice>(GetRef(m_VkInstance), GetScope(m_MainSurface));
		m_VkDevice			= MakeRef<VulkanDevice>(GetScope(m_VkPhysicalDevice));

		m_SwapChain = MakeScope<VulkanSwapchain>(m_VkDevice, GetScope(m_MainSurface), 
												 GetScope(m_VkPhysicalDevice), 
												 m_MainWindow->GetHandle());
		
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