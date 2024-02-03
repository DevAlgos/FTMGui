#pragma once

#include "VulkanPhysicalDevice.h"

namespace FTMGui {

	class VulkanDevice
	{
	public:
		VulkanDevice(const VulkanPhysicalDevice& PhysicalDevice);
		~VulkanDevice();

		inline const VkDevice GetHandle() const { return m_Device; }

		inline const VkQueue  GetGraphicsQueue() const { return m_GraphicsQueue; }
		inline const VkQueue  GetPresentQueue()  const { return m_PresentQueue; }

	private:
		VkDevice m_Device;

		VkQueue  m_GraphicsQueue;
		VkQueue  m_PresentQueue;
	};

}