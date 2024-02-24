#pragma once
#include <cinttypes>

#include "Window/Window.h"
#include "Events/EventDispatcher.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSurface.h"
#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanFence.h"
#include "Platform/Vulkan/VulkanSemaphore.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

#include <memory>

namespace FTMGui {

	enum class Platform { None = 0, Windows, Mac };

	struct AppDescriptor
	{
		Platform platform = Platform::Windows; //going to make redundant

		uint32_t windowWidth = 0;
		uint32_t windowHeight = 0;

		std::string_view appName = "App";
	};

	static constexpr uint32_t MaxFramesInFlight = 2;

	class FTMGuiContext
	{
	public:
		FTMGuiContext() = default;
		FTMGuiContext(Platform platform, const WindowInfo& info);

		~FTMGuiContext();

		void UpdateCtx();

		inline const bool IsRunning() const { return !m_MainWindow->WindowClosed(); }

	private:
		Ref<VulkanInstance> m_VkInstance;
		Ref<VulkanDevice> m_VkDevice;

		Platform m_Platform;
		uint32_t m_CurrentFrame = 0;

		Scope<Window>  m_MainWindow;
		Scope<VulkanSurface> m_MainSurface;

		Scope<VulkanPhysicalDevice> m_VkPhysicalDevice;
		Scope<VulkanSwapchain> m_SwapChain;

		Scope<VulkanRenderPass> m_RenderPass;
		Scope<VulkanPipeline> m_RenderPipeline;

		Scope<VulkanBuffer> m_VertexBuffer;

		std::vector<VulkanCommandBuffer> m_CommandBuffers;

		std::vector<VulkanFence> m_InFlightFences;
		std::vector<VulkanSemaphore> m_ImageAvailableSemaphores;
		std::vector<VulkanSemaphore> m_RenderFinishedSemaphores;

		std::vector<VulkanFramebuffer> m_Framebuffers;
	};

}