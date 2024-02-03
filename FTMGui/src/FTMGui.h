#pragma once
#include <cinttypes>

#include "Window/Window.h"
#include "Events/EventDispatcher.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/Vulkan/VulkanSurface.h"

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

		Platform m_Platform;

		Scope<Window>  m_MainWindow;
		Scope<VulkanSurface> m_MainSurface;

		Scope<VulkanPhysicalDevice> m_VkPhysicalDevice;
		Scope<VulkanDevice> m_VkDevice;
	};

}