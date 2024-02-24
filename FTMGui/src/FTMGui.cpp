#include "FTMGui.h"
#include "Utils/Log.h"

#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>

namespace FTMGui {
	FTMGuiContext::FTMGuiContext(Platform platform, const WindowInfo& info)
		: m_Platform(platform)
	{
		Log::Init();

		FTMGUI_LOG_INFO(std::filesystem::current_path());

		m_MainWindow = MakeScope<Window>(info);

		m_VkInstance = MakeRef<VulkanInstance>("FTMGui Application");

		m_MainSurface = MakeScope<VulkanSurface>(m_MainWindow->GetHandle(), m_VkInstance);

		m_VkPhysicalDevice	= MakeScope<VulkanPhysicalDevice>(GetRef(m_VkInstance), GetScope(m_MainSurface));
		m_VkDevice			= MakeRef<VulkanDevice>(GetScope(m_VkPhysicalDevice));

		m_SwapChain = MakeScope<VulkanSwapchain>(m_VkDevice, GetScope(m_MainSurface), 
												 GetScope(m_VkPhysicalDevice), 
												 m_MainWindow->GetHandle());
		
		m_RenderPass = MakeScope<VulkanRenderPass>(m_VkDevice, GetScope(m_SwapChain));

		std::unordered_map<ShaderType, std::filesystem::path> Shaders;
		Shaders[ShaderType::FragmentShader] = "FTMGui/src/Platform/Vulkan/frag.spv";
		Shaders[ShaderType::VertexShader] = "FTMGui/src/Platform/Vulkan/vert.spv";


		const std::vector<Vertex> vertices = {
			{{0.0f, -0.5f, 0.0f}, {0.4f, 0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.9f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
		};

		VulkanBufferDescription BufferDesc;
		BufferDesc.LogicalDevice = m_VkDevice;
		BufferDesc.PhysicalDevice = GetScope(m_VkPhysicalDevice);
		BufferDesc.Attributes = { Attribute::FLOAT_3, Attribute::FLOAT_3 };
		

		m_VertexBuffer = MakeScope<VulkanBuffer>(BufferDesc, 
			USAGE_VERTEX_BUFFER_BIT, MEMORY_HOST_VISIBLE_BIT | MEMORY_HOST_COHERENT_BIT, 
			vertices.size() * 6 * sizeof(float), glm::value_ptr(vertices[0].Position));

		m_RenderPipeline = MakeScope<VulkanPipeline>(m_VkDevice, GetScope(m_SwapChain), GetScope(m_RenderPass), Shaders, 
			m_VertexBuffer.get(), 1);
		
		m_Framebuffers.reserve(m_SwapChain->GetImageViews().size() + 1);
		for (uint32_t i = 0; i < m_SwapChain->GetImageViews().size(); i++)
			m_Framebuffers.emplace_back(m_VkDevice, GetScope(m_RenderPass), GetScope(m_SwapChain), i);

		m_CommandBuffers.reserve(MaxFramesInFlight);
		for (size_t i = 0; i < MaxFramesInFlight; i++)
			m_CommandBuffers.emplace_back(m_VkDevice, GetScope(m_VkPhysicalDevice), CREATE_RESET_COMMAND_BUFFER_BIT);

		m_InFlightFences.reserve(MaxFramesInFlight);
		for (size_t i = 0; i < MaxFramesInFlight; i++)
			m_InFlightFences.emplace_back(m_VkDevice);

		m_ImageAvailableSemaphores.reserve(MaxFramesInFlight);
		for (size_t i = 0; i < MaxFramesInFlight; i++)
			m_ImageAvailableSemaphores.emplace_back(m_VkDevice);

		m_RenderFinishedSemaphores.reserve(MaxFramesInFlight);
		for (size_t i = 0; i < MaxFramesInFlight; i++)
			m_RenderFinishedSemaphores.emplace_back(m_VkDevice);
	}

	FTMGuiContext::~FTMGuiContext()
	{
		glfwTerminate();
	}

	void FTMGuiContext::UpdateCtx()
	{
		m_MainWindow->Update();

		m_InFlightFences[m_CurrentFrame].Wait();

		uint32_t ImageIndex{};

		VkResult res = vkAcquireNextImageKHR(m_VkDevice->GetHandle(),
											 m_SwapChain->Get(), 
											 UINT64_MAX, 
											 m_ImageAvailableSemaphores[m_CurrentFrame].GetHandle(),
											 nullptr, 
											 &ImageIndex);

		if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
		{
			vkDeviceWaitIdle(m_VkDevice->GetHandle());

			m_SwapChain = nullptr;
			m_MainSurface = nullptr;


			m_MainSurface = MakeScope<VulkanSurface>(m_MainWindow->GetHandle(), m_VkInstance);
			m_VkPhysicalDevice->ReQuery(GetScope(m_MainSurface));
			m_SwapChain = MakeScope<VulkanSwapchain>(m_VkDevice, GetScope(m_MainSurface),
				GetScope(m_VkPhysicalDevice), m_MainWindow->GetHandle());

			m_Framebuffers.clear();
			m_Framebuffers.reserve(m_SwapChain->GetImageViews().size() + 1);
			for (uint32_t i = 0; i < m_SwapChain->GetImageViews().size(); i++)
				m_Framebuffers.emplace_back(m_VkDevice, GetScope(m_RenderPass), GetScope(m_SwapChain), i);
		}

		m_CommandBuffers[m_CurrentFrame].ResetCommandBuffer();

		m_RenderPass->BeginRenderPass(m_CommandBuffers[m_CurrentFrame], m_Framebuffers[ImageIndex],
			GetScope(m_SwapChain), GetScope(m_RenderPipeline), ImageIndex);

		m_VertexBuffer->Bind(m_CommandBuffers[m_CurrentFrame]);
		vkCmdDraw(m_CommandBuffers[m_CurrentFrame].GetHandle(), 3, 1, 0, 0);

		m_RenderPass->EndRenderPass(m_CommandBuffers[m_CurrentFrame]);


		VkSubmitInfo SubmitInfo{};
		SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		
		VkSemaphore WaitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame].GetHandle()};
		VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		SubmitInfo.waitSemaphoreCount = 1;
		SubmitInfo.pWaitSemaphores = WaitSemaphores;
		SubmitInfo.pWaitDstStageMask = WaitStages;

		VkCommandBuffer CmdBuffer[] = { m_CommandBuffers[m_CurrentFrame].GetHandle()};

		SubmitInfo.pCommandBuffers = CmdBuffer;
		SubmitInfo.commandBufferCount = 1;

		VkSemaphore SignalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame].GetHandle() };
		SubmitInfo.pSignalSemaphores = SignalSemaphores;
		SubmitInfo.signalSemaphoreCount = 1;

		vkCall(vkQueueSubmit, m_VkDevice->GetGraphicsQueue(), 1, &SubmitInfo, m_InFlightFences[m_CurrentFrame].GetHandle());

		VkPresentInfoKHR PresentInfo{};
		PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		PresentInfo.waitSemaphoreCount = 1;
		PresentInfo.pWaitSemaphores = SignalSemaphores;
		
		VkSwapchainKHR swapChains[] = { m_SwapChain->Get()};
		PresentInfo.swapchainCount = 1;
		PresentInfo.pSwapchains = swapChains;
		PresentInfo.pImageIndices = &ImageIndex;

		vkQueuePresentKHR(m_VkDevice->GetPresentQueue(), &PresentInfo);

		if (m_MainWindow->WindowClosed())
			vkDeviceWaitIdle(m_VkDevice->GetHandle());

		m_CurrentFrame = (m_CurrentFrame + 1) % MaxFramesInFlight;
	}

}