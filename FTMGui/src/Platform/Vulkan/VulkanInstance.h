#pragma once

#include "VulkanBase.h"

namespace FTMGui {

	class VulkanInstance
	{
	public:
		VulkanInstance(const std::string_view& ApplicationName);
		~VulkanInstance();

		inline const VkInstance GetHandle() const { return m_Instance; }

	private:
		std::vector<const char*> GetExtensions();

		void SetupDebugUtils();
		void DestroyDebugUtils();

	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}