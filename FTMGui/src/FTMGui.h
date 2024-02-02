#pragma once
#include <cinttypes>

#include "Window/Window.h"
#include "Events/EventDispatcher.h"

#include "Platform/Vulkan/VulkanPhysicalDevice.h"

#include <memory>

namespace FTMGui {

	enum class Platform { None = 0, Windows, Mac };

	struct AppDescriptor
	{
		Platform platform = Platform::Windows; //by default

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

		inline const bool IsRunning() const { return !m_MainWindow.WindowClosed(); }

	private:
		Window   m_MainWindow;
		Platform m_Platform;
		EventDispatcher m_EventDispatcher;

		Scope<VulkanInstance> m_VkInstance;
		Scope<VulkanPhysicalDevice> m_VkPhysicalDevice;
	};

}